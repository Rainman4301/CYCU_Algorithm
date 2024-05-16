#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>
#include <iterator>


#include <cmath>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <ctime>

#define IO_BUFFER_LEN 0x800
#define TABLE_MAX_ALLOW_SIZE 0x40000000 // 1 GiB

using namespace std;

//Functions to parse string.
namespace stringUtility {
	// To present the result of parsing.
	enum class intParseResult {
		success = 0,			// There is no error while parsing string to integer.
		intOverflow = 1,		// Integer overflow while parsing string to integer.
		noInput = 2,			// There is no any characters in input string.
		wrongFormat = 3,		// There contains a non-digit character in input string.
	};
	// Parse integer from string. success will be false if there have any error when prasing.
	// Allow number type: signed integer. Allow number format: 123 1,234 "123" '123' "1,234" '1,234' (123) (1,234)
	int tryParse_to_int32(string input, intParseResult& execResult) {
		char lastChar = '\0';
		bool haveInput = false;
		bool wrongFormat = false;
		bool intOverflow = false;
		bool negativeNum = false;
		char strStartToken = '\0';
		int outputNumber = 0;
		for (char readChar : input) {
			if (readChar == '\n') {
				break;
			}
			else if (readChar < 0) {
				wrongFormat = true;
				break;
			}
			else if (readChar == '"' || readChar == '\'') {
				if (strStartToken == '\0') {
					if (!haveInput) {
						strStartToken = readChar;
					}
					else {
						wrongFormat = true;
					}
				}
				else {
					break;
				}
			}
			else if (readChar == '(' && strStartToken == '\0') {
				if (!haveInput) {
					strStartToken = readChar;
				}
				else {
					wrongFormat = true;
				}
			}
			else if (readChar == ')' && strStartToken == '(') {
				break;
			}
			else if (haveInput && readChar == ',') {
				continue;
			}
			else if (isdigit(readChar)) {
				// multi number case
				if (isspace(lastChar) && outputNumber != 0) {
					wrongFormat = true;
				}
				if (lastChar == '-')
					negativeNum = true;
				if (outputNumber > 214748364) {
					intOverflow = true;
				}
				else if (outputNumber == 214748364 && (negativeNum ? (readChar - '0') > 8 : (readChar - '0') > 7)) {
					intOverflow = true;
				}
				outputNumber *= 10;
				outputNumber += (readChar - '0');
				haveInput = true;
			}
			else if (lastChar == '-') {
				if (haveInput) {
					wrongFormat = true;
				}
			}
			else if (!isspace(readChar) && readChar != '-') {
				wrongFormat = true;
			}
			if (wrongFormat || intOverflow)
				break;
			lastChar = readChar;
		}
		if (!haveInput)
			execResult = intParseResult::noInput;
		else if (intOverflow)
			execResult = intParseResult::intOverflow;
		else if (wrongFormat)
			execResult = intParseResult::wrongFormat;
		else
			execResult = intParseResult::success;
		if (negativeNum)
			outputNumber = -outputNumber;
		return (haveInput && !wrongFormat && !intOverflow) ? outputNumber : 0;
	}
	// Allow boolean format: true false (case insensitive)
	bool tryParseToBoolean(string input, bool& parseResult) {
		string Compare_str;
		bool output = false;
		for (char CompareChar : input) {
			if (CompareChar == ' ' && Compare_str.size() == 0)
				continue;
			if ('A' <= CompareChar && CompareChar <= 'Z') {
				Compare_str += (CompareChar - 'A' + 'a');
			}
			else {
				Compare_str += CompareChar;
			}
		}
		if (Compare_str.size() == 4) {
			if (Compare_str == "true") {
				parseResult = true;
				output = true;
			}
			else {
				parseResult = false;
			}
		}
		else if (Compare_str.size() == 5) {
			if (Compare_str == "false") {
				parseResult = true;
				output = false;
			}
			else {
				parseResult = false;
			}
		}
		return output;
	}
	double tryParseToDouble(string input, bool& parseResult) {
		// 1 11 52
		bool haveInput = false;
		bool isNegative = false;
		bool isReadDot = false;
		bool wrongFormat = false;

		double outputBeforeDot = 0.0;
		double outputAfterDot = 0.0;
		double outputAfterDotDiv = 1.0;
		double output = 0.0f;
		char prevCh = '\0';

		for (char ch : input) {
			if (wrongFormat)
				break;
			if (ch == '-' && !haveInput && !isNegative) {
				isNegative = true;
			}
			else if (isdigit(ch)) {
				if (prevCh == '.') {
					if(!isReadDot)
						isReadDot = true;
					else {
						wrongFormat = true;
						continue;
					}
				}
				int digit = ch - '0';
				if (isReadDot) {
					outputAfterDot = ((outputAfterDot * 10.0) + (digit));
					outputAfterDotDiv *= 10;
				}
				else {
					outputBeforeDot = ((outputBeforeDot * 10.0) + (digit));
				}
				haveInput = true;
			}
			else if (isblank(ch)) {

			}
			else if (ch == '.') {

			}
			else {
				wrongFormat = true;
			}
			prevCh = ch;
		}
		if (wrongFormat) {
			output = (double)NAN;
			parseResult = false;
		}
		else {
			output = outputBeforeDot + (outputAfterDot / outputAfterDotDiv);
		}
		return output;
	}
	// Formating string with C-style format.
	string formatString(const char* format, ...) {
		char buffer[300];
		va_list vl;
		va_start(vl, format);
		vsnprintf(buffer, sizeof(buffer), format, vl);
		va_end(vl);
		return string(buffer);
	}
	// Convert int to string. For compatibility.
	string toString(int value) {
		string output_str;
		if (value == 0x80000000)
			return string("-2147483648");
		if (value < 0) {
			value *= -1;
			output_str += '-';
		}
		int DivNum = 1;
		while ((value / DivNum) >= 10) {
			DivNum *= 10;
		}
		do {
			output_str += (char)((value / DivNum) + '0');
			value = value % DivNum;
			DivNum /= 10;
		} while (DivNum);
		return output_str;
	}
	// Convert float to strange format.
	string toString(float value, int totalWidth) {
		int aPart = (int)value;
		int bPart = (int)(round(value * 100)) % 100;
		int precision = 2;
		while (precision > 0 && (bPart % 10) == 0) {
			bPart /= 10;
			precision--;
		}
		if (bPart == 0) {
			return formatString("%*d", totalWidth, aPart);
		}
		else {
			return formatString("%*d.%*d", totalWidth - precision - 1,aPart, precision, bPart);
		}
	}
}
//Functions to read what user types from stdin.
namespace stdioUtility {
	// Read a signed 32 bits integer from stdin.
	// If there have any errors while parsing, this function will print the reason.
	// eofToken is means that stdin stream was arrived the end.
	int readInt32(bool& success, bool& eofToken) {
		char readChar = (char)fgetc(stdin);
		char lastChar = '\0';
		bool haveInput = false;
		bool wrongFormat = false;
		bool intOverflow = false;
		bool negativeNum = false;
		int outputNumber = 0;
		while (readChar != '\n' && readChar != EOF) {
			if (isdigit(readChar)) {
				// multi number case
				if (isspace(lastChar) && outputNumber != 0)
					wrongFormat = true;
				if (lastChar == '-')
					negativeNum = true;
				if (outputNumber > 214748364) {
					intOverflow = true;
				}
				else if (outputNumber == 214748364 && (negativeNum ? (readChar - '0') > 8 : (readChar - '0') > 7)) {
					intOverflow = true;
				}
				outputNumber *= 10;
				outputNumber += (readChar - '0');
				haveInput = true;
			}
			else if (lastChar == '-') {
				if (haveInput)
					wrongFormat = true;
			}
			else if (!isspace(readChar)) {
				wrongFormat = true;
			}
			lastChar = readChar;
			readChar = (char)fgetc(stdin);
		}
		if (wrongFormat) {
			printf("*** ERROR: Wrong Format. ***\n");
		}
		else if (intOverflow) {
			printf("*** ERROR: Int Overflow ***\n");
		}
		if (negativeNum)
			outputNumber = -outputNumber;
		eofToken = readChar == EOF;
		success = haveInput && !wrongFormat && !intOverflow;
		return success ? outputNumber : 0;
	}
	// Read string from stdin.
	// All strings that over then 200 bytes is not accepted.
	string readString(bool& success, bool& eofToken) {
		string out_str = "";
		char readChar = (char)fgetc(stdin);
		bool overSize = false;
		bool nonAscii = false;
		while (readChar != '\n' && readChar != EOF) {
			if (overSize || out_str.size() >= 200) {
				overSize = true;
			}
			else if ((readChar >> 7) != 0) {
				nonAscii = true;
			}
			out_str += readChar;
			readChar = (char)fgetc(stdin);
		}
		eofToken = readChar == EOF;
		if (overSize) {
			printf("*** ERROR: the length of input string cannot over than 200 ***\n");
		}
		else if (nonAscii) {
			printf("*** ERROR: there is non-ascii character in input string. ***\n");
		}
		success = !overSize && !nonAscii;
		return success ? out_str : "";
	}
}
//Functions that provides some file operating method which is not provided by C++ std library.
namespace ioUtility {
	// Check whether if the file is exist or not.
	bool fileExists(const string& file_path) {
		ifstream stream;
		stream.open(file_path, ios_base::in);
		bool checkResult = stream.good();
		stream.close();
		return checkResult;
	}

	class binaryReader {
	public:
		uint8_t readByte() {
			uint8_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		int8_t readSByte() {
			int8_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		char readChar() {
			char outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}

		uint16_t readUInt16() {
			uint16_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		int16_t readInt16() {
			int16_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}

		uint32_t readUInt32() {
			uint32_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		int32_t readInt32() {
			int32_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}

		uint64_t readUInt64() {
			uint64_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		int64_t readInt64() {
			int64_t outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}

		size_t getReadableLength() {
			return getPosition() - getLength();
		}

		virtual size_t readChars(char* target, size_t length) {}
		virtual size_t getPosition() {}
		virtual size_t getLength() {}

		virtual bool isEnd() {}
	};
	class memoryBinaryReader: public binaryReader {
	public:
		memoryBinaryReader(const char* _baseArray, size_t _length) {
			baseArrayPtr = _baseArray;
			position = 0;
			length = _length;
		}

		size_t readChars(char* target, size_t readLength) {
			size_t readSize = min(getReadableLength(), readLength);
			memcpy(target, baseArrayPtr + position, readLength);
			position += readSize;
			return readSize;
		}

		size_t getPosition() { return position; }
		size_t getLength() { return length; }

		bool isEnd() { return position >= length; }

	private:
		const char* baseArrayPtr;
		size_t position;
		size_t length;
	};
	class streamBinaryReader : public binaryReader {
	public:
		streamBinaryReader(istream& inputStream):baseStream(inputStream){
			position = 0;
			length = 0;
		}

		size_t readChars(char* target, size_t readLength) {
			baseStream.read(target, readLength);
			size_t readSize = baseStream.gcount();
			position += readSize;
			return readSize;
		}

		size_t getPosition() { return position; }
		size_t getLength() { return 0; }

		bool isEnd() { return baseStream.eof(); }

	private:
		istream& baseStream;
		size_t position;
		size_t length;
	};

	class binaryWriter {
	public:
		void writeByte(uint8_t value) {
			writeChars((char*)(&value), sizeof(value));
		}
		void writeSByte(int8_t value) {
			writeChars((char*)(&value), sizeof(value));
		}

		void writeUInt16(uint16_t value) {
			writeChars((char*)(&value), sizeof(value));
		}
		void writeInt16(int16_t value) {
			writeChars((char*)(&value), sizeof(value));
		}

		void writeUInt32(uint32_t value) {
			writeChars((char*)(&value), sizeof(value));
		}
		void writeInt32(int32_t value) {
			writeChars((char*)(&value), sizeof(value));
		}

		void writeUInt64(uint64_t value) {
			writeChars((char*)(&value), sizeof(value));
		}
		void writeInt64(int64_t value) {
			writeChars((char*)(&value), sizeof(value));
		}

		void writeFloat(float value) {
			writeChars((char*)(&value), sizeof(value));
		}
		void writeDouble (double value) {
			writeChars((char*)(&value), sizeof(value));
		}

		virtual void writeChars(const char* source, size_t length) {}
		virtual size_t getLength() { return 0; }
	};

	class streamBinaryWriter : public binaryWriter {
	public:
		streamBinaryWriter(ostream& outStream) : baseStream(outStream) {
			length = 0;
		}

		void writeChars(const char* source, size_t writeLength) {
			baseStream.write(source, writeLength);
			length += writeLength;
		}

		size_t getLength() {
			return length;
		}
	private:
		ostream& baseStream;
		size_t length;
	};
}
//Classes that provides the implementation of timer.
namespace timerUtility {
	// Class that provides the all timer exception
	class timerException : exception {
	public:
		// Constructors
		timerException() : exception(), exception_msg("") {

		}
		timerException(const char* Exception_msg) : exception_msg(Exception_msg) {

		}
		// Overloading virutal functions
		const char* what() const noexcept {
			return exception_msg;
		}
	private:
		const char* exception_msg;
	};
	class timer {
	public:
		// Constructor of timer
		timer() : enabled(false), startCpuClock(-1), endCpuClock(-1) {
		}
		// Start timer.
		void start() {
			enabled = true;
			startCpuClock = clock();
		}
		// Stop timer.
		void stop() {
			endCpuClock = clock();
			enabled = false;
		}
		// Get duration
		float getDuration() const {
			if (enabled) {
				throw timerException("At <" "timer::getDuration" "> : This timer is running. Please ensure this timer was stop.");
			}
			else if (startCpuClock == -1) {
				throw timerException("At <" "timer::getDuration" "> : This timer is not start yet.");
			}
			float duration = (float)(endCpuClock - startCpuClock) / CLOCKS_PER_SEC;
			return duration * 1000;
		}
	private:
		clock_t startCpuClock;
		clock_t endCpuClock;
		bool enabled;
	};
}
//Classes that provides the implementation of CSV-style table reading and writing.
namespace tableUtility {
	// It will be thrown if there have any exception when processing csv-style table.
	class tableException : exception {
	public:
		// Constructors
		tableException() : exception(), exception_msg("") {

		}
		tableException(const char* Exception_msg) : exception_msg(Exception_msg) {

		}
		// Overloading virutal functions
		const char* what() const noexcept {
			return exception_msg;
		}
	private:
		const char* exception_msg;
	};
	// To present a object in table.
	class tableObject {
	public:
		// Constructors
		tableObject() : baseString(string("")) {

		}
		tableObject(const string& sourceString) {
			setValue(sourceString);
		}
		tableObject(int sourceInteger) {
			setValue(sourceInteger);
		}
		tableObject(bool sourceBoolean) {
			setValue(sourceBoolean);
		}
		// Set methods
			// Set the value of this table object.
		void setValue(const string& value) {
			baseString = value;
		}
		void setValue(int value) {
			baseString = stringUtility::toString(value);
		}
		void setValue(const bool& value) {
			baseString = value ? string("TRUE") : string("FALSE");
		}
		// Get methods
		// Regard this table object as int type, if this object cannot be converted into int, it will throw the exception.
		int as_int() const {
			stringUtility::intParseResult convertResult;
			int convertedValue = stringUtility::tryParse_to_int32(baseString, convertResult);
			if (convertResult != stringUtility::intParseResult::success && convertResult != stringUtility::intParseResult::noInput) {
				throw tableException("At <" "tableObject::as_int" "> : This tableObject cannot be integer.");
			}
			else {
				return convertedValue;
			}
		}
		// Regard this table object as int type, if this object cannot be converted into int, it will set <result> argument to false.
		int as_int(bool& result) const {
			stringUtility::intParseResult convertResult;
			int convertedValue = stringUtility::tryParse_to_int32(baseString, convertResult);
			if (convertResult != stringUtility::intParseResult::success && convertResult != stringUtility::intParseResult::noInput) {
				result = false;
			}
			else {
				return convertedValue;
			}
		}
		// Regard this table object as int type, if this object cannot be converted into int, it will return 0;
		int as_int_nochk() const {
			stringUtility::intParseResult convertResult;
			int convertedValue = stringUtility::tryParse_to_int32(baseString, convertResult);
			return convertedValue;
		}
		// Regard this table object as string.
		string asString() const {
			return baseString;
		}
		// Regard this table object as boolean type.
		bool asBoolean() const {
			bool BoolParseResult;
			bool convertedValue = stringUtility::tryParseToBoolean(baseString, BoolParseResult);
			if (!BoolParseResult) {
				throw tableException("At <" "tableObject::asBoolean" "> : This tableObject cannot be boolean.");
			}
			else {
				return convertedValue;
			}
		}
		// Regard this table object as boolean type.
		bool asBoolean(bool& result) const {
			bool convertedValue = stringUtility::tryParseToBoolean(baseString, result);
			return convertedValue;
		}
		// Regard this table object as boolean type.
		bool asBoolean_nochk() const {
			bool BoolParseResult;
			bool convertedValue = stringUtility::tryParseToBoolean(baseString, BoolParseResult);
			return convertedValue;
		}
		// Regard this table object as float type.
		float asFloat() const {
			bool FloatParseResult;
			float convertedValue = (float)stringUtility::tryParseToDouble(baseString, FloatParseResult);
			if (!FloatParseResult) {
				throw tableException("At <" "tableObject::asBoolean" "> : This tableObject cannot be boolean.");
			}
			else {
				return convertedValue;
			}
		}
		// Regard this table object as boolean type.
		float asFloat(bool& result) const {
			float convertedValue = (float)stringUtility::tryParseToDouble(baseString, result);
			return convertedValue;
		}
		// Regard this table object as boolean type.
		float asFloat_nochk() const {
			bool FloatParseResult;
			float convertedValue = (float)stringUtility::tryParseToDouble(baseString, FloatParseResult);
			return convertedValue;
		}
	private:
		//Members
		string baseString;			// The base string of table object.
	};
	// To present a row in table.
	class tableRow {
	public:
		// Classes
		class readIndicator {
		public:
			readIndicator(const tableRow& baseRow) : baseTableRow(baseRow), position(0) {

			}
			readIndicator operator ++ (int) const {
				readIndicator __readIndicator = (*this);
				if (!isEnd())
					__readIndicator.position++;
				return __readIndicator;
			}
			readIndicator& operator ++ () {
				readIndicator& __readIndicator = (*this);
				if (!isEnd())
					__readIndicator.position++;
				return __readIndicator;
			}
			readIndicator operator >> (int& value) const {
				if (isEnd()) {
					value = 0;
				}
				else {
					value = baseTableRow[position].as_int_nochk();
				}
				readIndicator __readIndicator = (*this);
				return __readIndicator++;
			}
			readIndicator operator >> (string& value) const {
				if (isEnd()) {
					value = "";
				}
				else {
					value = baseTableRow[position].asString();
				}
				readIndicator __readIndicator = (*this);
				return __readIndicator++;
			}
			readIndicator operator >> (bool& value) const {
				if (isEnd()) {
					value = false;
				}
				else {
					value = baseTableRow[position].asBoolean_nochk();
				}
				readIndicator __readIndicator = (*this);
				return __readIndicator++;
			}
		private:
			// Members
			const tableRow& baseTableRow;
			int position;
			// Methods
			bool isEnd() const {
				return position >= baseTableRow.getColumnCount();
			}
		};
		// Constructors
		tableRow() : baseRowContainer(vector<tableObject>()), columnCount(0) {
		}
		// Set methods
		// Set a column to a number given at argument.
		void setColumn(int columnOffset, int newValue) {
			setColumn(columnOffset, tableObject(newValue));
		}
		// Set a column to a string given at argument.
		void setColumn(int columnOffset, const string& newValue) {
			setColumn(columnOffset, tableObject(newValue));
		}
		void setColumn(int columnOffset, const tableObject& newValue) {
			if (columnOffset >= getColumnCount()) {
				baseRowContainer.resize(columnOffset, tableObject());
			}
			baseRowContainer[columnOffset] = newValue;
			columnCount = (int)baseRowContainer.size();
		}
		// Get methods
		const tableObject& getColumn(int columnOffset) const {
			if (columnOffset >= getColumnCount()) {
				throw tableException("At <" "tableRow::getColumn" "> : columnOffset is greater than or equal to column count.");
			}
			else {
				return baseRowContainer[columnOffset];
			}
		}
		const tableObject& getColumn(int columnOffset, int defaultValue) const {
			if (columnOffset >= getColumnCount()) {
				return tableObject(defaultValue);
			}
			else {
				return baseRowContainer[columnOffset];
			}
		}
		const tableObject& getColumn(int columnOffset, const string& defaultValue) const {
			if (columnOffset >= getColumnCount()) {
				return tableObject(defaultValue);
			}
			else {
				return baseRowContainer[columnOffset];
			}
		}
		const tableObject& getColumn(int columnOffset, const tableObject& defaultValue) const {
			if (columnOffset >= getColumnCount()) {
				return defaultValue;
			}
			else {
				return baseRowContainer[columnOffset];
			}
		}
		int getColumnCount() const {
			return (int)columnCount;
		}
		// Column operating methods
		void insertColumn(int columnOffset, int newObject) {
			if (columnOffset >= getColumnCount()) {
				baseRowContainer.resize((size_t)columnOffset, tableObject());
			}
			else {
				baseRowContainer.push_back(tableObject());
				for (int i = getColumnCount(); i > columnOffset; i--) {
					baseRowContainer[i] = baseRowContainer[i - 1];
				}
				baseRowContainer[columnOffset] = tableObject(newObject);
			}
			columnCount = (int)baseRowContainer.size();
		}
		void insertColumn(int columnOffset, const string& newObject) {
			if (columnOffset >= getColumnCount()) {
				baseRowContainer.resize((size_t)columnOffset, tableObject());
			}
			else {
				baseRowContainer.push_back(tableObject());
				for (int i = getColumnCount(); i > columnOffset; i--) {
					baseRowContainer[i] = baseRowContainer[i - 1];
				}
				baseRowContainer[columnOffset] = tableObject(newObject);
			}
			columnCount = (int)baseRowContainer.size();
		}
		void insertColumn(int columnOffset, const tableObject& newObject) {
			if (columnOffset >= getColumnCount()) {
				baseRowContainer.resize((size_t)columnOffset, tableObject());
			}
			else {
				baseRowContainer.push_back(tableObject());
				for (int i = getColumnCount(); i > columnOffset; i--) {
					baseRowContainer[i] = baseRowContainer[i - 1];
				}
				baseRowContainer[columnOffset] = newObject;
			}
			columnCount = (int)baseRowContainer.size();
		}
		void appendColumn(int newObject) {
			baseRowContainer.push_back(tableObject(newObject));
			columnCount = (int)baseRowContainer.size();
		}
		void appendColumn(const string& newObject) {
			baseRowContainer.push_back(tableObject(newObject));
			columnCount = (int)baseRowContainer.size();
		}
		void appendColumn(const tableObject& newObject) {
			baseRowContainer.push_back(newObject);
			columnCount = (int)baseRowContainer.size();
		}
		void clear() {
			baseRowContainer.clear();
			columnCount = (int)baseRowContainer.size();
		}
		// Overloading operators
		const tableObject& operator [](int columnOffset) const {
			if (columnOffset >= baseRowContainer.size())
				return tableObject();
			return baseRowContainer[columnOffset];
		}
		tableObject& operator [](int columnOffset) {
			return baseRowContainer[columnOffset];
		}
		tableRow& operator <<(const int& newObject) {
			baseRowContainer.push_back(tableObject(newObject));
			columnCount = (int)baseRowContainer.size();
			return *(this);
		}
		tableRow& operator <<(const string& newObject) {
			baseRowContainer.push_back(tableObject(newObject));
			columnCount = (int)baseRowContainer.size();
			return *(this);
		}
		tableRow& operator <<(const bool& newObject) {
			baseRowContainer.push_back(tableObject(newObject));
			columnCount = (int)baseRowContainer.size();
			return *(this);
		}
		tableRow& operator <<(const tableObject& newObject) {
			baseRowContainer.push_back(newObject);
			columnCount = (int)baseRowContainer.size();
			return *(this);
		}
		tableRow::readIndicator operator >>(int& value) const {
			readIndicator indicator = readIndicator(*this);
			return indicator >> value;
		}
		tableRow::readIndicator operator >>(string& value) const {
			readIndicator indicator = readIndicator(*this);
			return indicator >> value;
		}
		tableRow::readIndicator operator >>(bool& value) const {
			readIndicator indicator = readIndicator(*this);
			return indicator >> value;
		}
	private:
		// Members
		vector<tableObject> baseRowContainer;

		int columnCount;
	};
	// To present a table instance.
	class table {
	public:
		// Constructors
		table() : baseContainer(vector<tableRow>()) {
			baseContainer.push_back(tableRow());
		}
		// File processing methods
		void readFromFile(const string& filename, char separator, bool withHeader, int maxAllowSize = TABLE_MAX_ALLOW_SIZE) {
			if (!ioUtility::fileExists(filename)) {
				throw tableException("At <" "table::readFromFile" "> : file cannot be found.");
			}
			else {
				ifstream fileStream;
				fileStream.open(filename);
				readFromStream(fileStream, separator, withHeader, maxAllowSize);
				fileStream.close();
			}
		}
		void readFromStream(istream& baseStream, char separator, bool withHeader, int maxAllowSize = TABLE_MAX_ALLOW_SIZE) {
			char _readBuffer[IO_BUFFER_LEN];
			parse_state Current_state = parse_state::standardString;
			tableRow OutputRow;
			string _strBuffer;
			string BlankBuffer;

			char _prevCh = '\0';

			while (!baseStream.eof()) {
				baseStream.read(_readBuffer, IO_BUFFER_LEN);
				int _readCount = (int)baseStream.gcount();
				if (_readCount < IO_BUFFER_LEN) {
					_readBuffer[_readCount++] = '\0';
				}
				for (int i = 0; i < _readCount; i++) {
					char Ch = _readBuffer[i];
					char _nextch = (i + 1) < _readCount ? _readBuffer[i + 1] : '\0';
					switch (Current_state) {
					case parse_state::standardString:
						if (Ch == '"' && _strBuffer.size() == 0) {
							Current_state = parse_state::bracketString;
							_prevCh = '\0';
						}
						else if (Ch == '\r' || Ch == '\0' || (Ch == '\n' && _prevCh != '\r')) {
							OutputRow << _strBuffer;
							baseContainer.push_back(OutputRow);
							OutputRow.clear();
							_strBuffer.clear();
							BlankBuffer.clear();
						}
						else if (Ch == separator) {
							OutputRow << _strBuffer;
							_strBuffer.clear();
							BlankBuffer.clear();
						}
						else if (Ch == ' ') {
							if (_strBuffer.size() > 0)
								BlankBuffer += ' ';
						}
						else {
							if (BlankBuffer.size() > 0) {
								_strBuffer += BlankBuffer;
								BlankBuffer.clear();
							}
							_strBuffer += Ch;
						}
						break;
					case parse_state::bracketString:
						if (Ch == '"') {
							if (_nextch != '"') {
								Current_state = parse_state::standardString;
							}
							else {
								_strBuffer += '"';
								i++;
							}
						}
						else if (Ch == '\r' || (Ch == '\n' && _prevCh != '\r')) {
							_strBuffer += '\n';
						}
						else if (_nextch == '\0') {
							Current_state = parse_state::standardString;
							_prevCh = '\0';
						}
						else {
							_strBuffer += Ch;
						}
						break;
					}
					_prevCh = Ch;
				}
			}
			if (withHeader && baseContainer.size() > 1)
				baseContainer.erase(baseContainer.begin());
		}
		void saveToFile(const string& filename, char separator, bool withHeader, bool autofit = false) {
			ofstream fileStream;
			fileStream.open(filename);
			saveToStream(fileStream, separator, withHeader, autofit);
			fileStream.close();
		}
		void saveToStream(ostream& baseStream, char separator, bool withHeader, bool autofit = false) {
			int _initial_index = withHeader ? 0 : 1;
			if (autofit) {
				vector<int> _max_width;
				for (int i = _initial_index; i < baseContainer.size(); i++) {
					const tableRow& OutputRow = baseContainer[i];
					for (int j = 0; j < OutputRow.getColumnCount(); j++) {
						int _str_len = OutputRow.getColumn(j).asString().size();
						if (_max_width.size() <= j) {
							_max_width.push_back(_str_len);
						}
						else {
							if (_max_width[j] < _str_len)
								_max_width[j] = _str_len;
						}
					}
				}
				for (int i = _initial_index; i < baseContainer.size(); i++) {
					const tableRow& OutputRow = baseContainer[i];
					if (i > _initial_index)
						baseStream << '\n';
					for (int j = 0; j < OutputRow.getColumnCount(); j++) {
						if (j)
							baseStream << separator;
						string output_str = OutputRow.getColumn(j).asString();
						output_str.resize(_max_width[j], ' ');
						baseStream << output_str;
					}
				}
			}
			else {
				for (int i = _initial_index; i < baseContainer.size(); i++) {
					const tableRow& OutputRow = baseContainer[i];
					if (i > _initial_index)
						baseStream << '\n';
					for (int j = 0; j < OutputRow.getColumnCount(); j++) {
						if (j)
							baseStream << separator;
						baseStream << OutputRow.getColumn(j).asString();
					}
				}
			}
		}
		// Set methods
		void setRow(int rowOffset, const tableRow& newValue) {
			rowOffset++;
			if (rowOffset >= baseContainer.size()) {
				throw tableException("At <" "table:::setRow" "> : Given rowOffset is invalid.");
			}
			else {
				baseContainer[rowOffset] = newValue;
			}
		}
		void setHeader(const tableRow& newHeader) {
			baseContainer[0] = newHeader;
		}
		// Get methods
		tableRow getRow(int rowOffset) const {
			rowOffset++;
			if (rowOffset >= baseContainer.size()) {
				throw tableException("At <" "table::getRow" "> : Given rowOffset is invalid.");
			}
			else {
				return baseContainer[rowOffset];
			}
		}
		tableRow getHeader() const {
			return baseContainer[0];
		}
		int getRowCount() const {
			return (int)baseContainer.size() - 1;
		}
		// Row operating
		void insertRow(int rowOffset, const tableRow& newRow) {
			rowOffset++;
			if (rowOffset >= baseContainer.size()) {
				baseContainer.resize(rowOffset + 1, tableRow());
				baseContainer[rowOffset] = newRow;
			}
			else {
				baseContainer.push_back(tableRow());
				for (int i = baseContainer.size() - 1; i > rowOffset; i--) {
					baseContainer[i] = baseContainer[i - 1];
				}
				baseContainer[rowOffset] = newRow;
			}
		}
		// Overloading Operators
		const tableRow& operator [](int rowOffset) const {
			rowOffset++;
			if (rowOffset >= baseContainer.size()) {
				throw tableException("At <" "table::operator[]" "> : Given rowOffset is invalid.");
			}
			else {
				return baseContainer[rowOffset];
			}
		}
		tableRow& operator [](int rowOffset) {
			rowOffset++;
			if (rowOffset >= baseContainer.size()) {
				throw tableException("At <" "table::operator[]" "> : Given rowOffset is invalid.");
			}
			else {
				return baseContainer[rowOffset];
			}
		}
		table& operator << (const tableRow& newRow) {
			baseContainer.push_back(newRow);
			return *(this);
		}
	private:
		// Members
		vector<tableRow> baseContainer;
		// Enum Classes
		enum class parse_state {
			none, standardString, bracketString
		};
	};
}
//Classes that provides some utility of math
namespace mathUtility {
	class primeTable {
	public:
		primeTable(int _maxNumber) {
			maxNumber = _maxNumber;
			createTable();
		}

		int getNearbyLess(int foundNum) {
			int start = 0;
			int end = primes.size() - 1;
			while (start < (end - 1)) {
				int mid = (start + end) >> 1;
				if (primes[mid] < foundNum) {
					start = mid;
				}
				else if (primes[mid] == foundNum) {
					return foundNum;
				}
				else {
					end = mid - 1;
				}
			}
			if (primes[end] <= foundNum)
				return primes[end];
			else
				return primes[start];
		}
		int getNearbyMax(int foundNum) {
			int start = 0;
			int end = primes.size() - 1;
			while (start < (end - 1)) {
				int mid = (start + end) >> 1;
				if (primes[mid] < foundNum) {
					start = mid + 1;
				}
				else if (primes[mid] == foundNum) {
					return foundNum;
				}
				else {
					end = mid;
				}
			}
			if (primes[start] >= foundNum)
				return primes[start];
			else
				return primes[end];
		}
	private:
		vector<int> primes;
		int maxNumber;
		void createTable() {
			vector<bool> isPrimeTable((maxNumber >> 1) + 1, true);
			primes.push_back(2);
			for (int i = 3; i <= maxNumber; i += 2) {
				if (isPrimeTable[(i >> 1) - 1]) {
					for (int j = i * 3; j <= maxNumber; j += (i << 1))
						isPrimeTable[(j >> 1) - 1] = false;
					primes.push_back(i);
				}
			}
		}
	};
}
//
namespace hashTableUtility {
	template <typename TKey, typename TValue> 
	class keyValuePair {
	public:
		// Members
		TKey key;
		TValue value;
		// Constructor
		keyValuePair(const TKey& _key, const TValue& _value) {
			key = _key;
			value = _value;
		}
	};
	
	/*	
		Type: hashFunc	
			
	*/
	template<typename TKey>
	using hashFunc = uint32_t(*)(const size_t hashTableSize, const TKey& key);

	template<typename TKey>
	using compareFunc = int(*)(const TKey& key1, const TKey& key2);


	/*
		Abstract Class: hashTable
			TKey:	the key type of the keyValuePair hashTable stored.
			TValue:	the value type of the keyValuePair hashTable stored.

		It cannot construct directly, because it is abstract class.

	*/
	template<typename TKey, typename TValue>
	class hashTable {
	public:
		~hashTable() {
			while (containerSize > 0) {
				if (isOccupied(containerSize - 1))
					delete elementPtrContainer[containerSize - 1];
				containerSize--;
			}
		}
		// Basic operations
		
		// Add the new key-value pair into hashTable 
		virtual void add(const TKey& key, const TValue& value) {
			if (isFull())
				throw runtime_error("full");
			size_t index = getPushLocation(key);
			if(!isNeverused(index))
				throw runtime_error("depulicate");
			if (index >= containerSize)
				throw runtime_error("overrange");
			elementPtrContainer[index] = new keyValuePair<TKey, TValue>(key, value);
			elementCount++;
		}

		// Remove the item by giving the key you want to remove.
		virtual bool remove(const TKey& key) {
			size_t index = getItemLocation(key);
			if (index < 0 || index >= containerSize)
				return false;
			if (isAvailable(index))
				return false;
			keyValuePair<TKey, TValue>* delItemPtr = elementPtrContainer[index];
			if (delItemPtr == &deletedKeyValuePair)
				return false;
			delete delItemPtr;
			elementPtrContainer[index] = &deletedKeyValuePair;
			elementCount--;
			return true;
		}
		virtual bool contains(const TKey& key) const {
			size_t index = getItemLocation(key);
			if (index < 0 || index >= containerSize)
				return false;
			if (isAvailable(index))
				return false;
			return true;
		}
		
		// Getter
		TValue& getValue(const TKey& key) {
			size_t index = getItemLocation(key);
			if(index >= containerSize || index < 0)
				throw runtime_error("not found");
			return elementPtrContainer[index]->value;
		}
		TValue getValue(const TKey& key) const {
			size_t index = getItemLocation(key);
			if (index >= containerSize || index < 0)
				throw runtime_error("not found");
			return TValue(elementPtrContainer[index]->value);
		}
		size_t getContainerSize() const{
			return containerSize;
		}
		keyValuePair<TKey,TValue>& getContainerItem(size_t index){
			return *elementPtrContainer[index];
		}
		const keyValuePair<TKey, TValue>& getContainerItem(size_t index) const{
			return *elementPtrContainer[index];
		}
		bool isNeverused(size_t index) const{
			if (index >= containerSize)
				return false;
			return elementPtrContainer[index] == NULL;
		}
		bool isDeleted(size_t index) const {
			if (index >= containerSize)
				return false;
			return elementPtrContainer[index] == &deletedKeyValuePair;
		}
		bool isAvailable(size_t index) const {
			return isNeverused(index) || isDeleted(index);
		}
		bool isOccupied(size_t index) const {
			return !isNeverused(index) && !isDeleted(index);
		}

		size_t getCount() const {
			return elementCount;
		}
		bool isEmpty() const {
			return elementCount == 0;
		}
		bool isFull() const {
			return elementCount == containerSize;
		}

	protected:
		vector< keyValuePair<TKey, TValue>* > elementPtrContainer;

		hashTable(size_t _tableSize) : deletedKeyValuePair(keyValuePair<TKey, TValue>(TKey(), TValue())) {
			elementPtrContainer = vector< keyValuePair<TKey, TValue>* >(_tableSize, NULL);
			containerSize = _tableSize;
			elementCount = 0;
		}
		hashTable(const hashTable& copySrc) {
			elementPtrContainer = new keyValuePair<TKey, TValue>* [containerSize];
			elementCount = copySrc.elementCount;
			containerSize = copySrc.containerSize;
			for (size_t i = 0; i < containerSize; i++)
				elementPtrContainer[i] = new keyValuePair<TKey, TValue>(*copySrc.elementPtrContainer[i]);
		}
		hashTable(hashTable&& moveSrc) {
			elementPtrContainer = std::move(moveSrc.elementPtrContainer);
			elementCount = moveSrc.elementCount;
			containerSize = moveSrc.containerSize;
			moveSrc.elementCount = -1;
			moveSrc.containerSize = -1;
		}


		virtual size_t getPushLocation(const TKey& item) const {
			return -1;
		}
		virtual size_t getItemLocation(const TKey& item) const {
			return -1;
		}
	private:
		size_t elementCount;
		size_t containerSize;
		keyValuePair<TKey, TValue> deletedKeyValuePair; // Used for identifing whether the item was deleted or not. 
	};

	template<typename TKey, typename TValue>
	class quadProbHashTable : public hashTable<TKey, TValue>{
	public:
		quadProbHashTable(size_t _tableSize, hashFunc<TKey> _hashFunction, compareFunc<TKey> _compareFunction) : hashTable<TKey, TValue>(_tableSize) {
			hashFunction = _hashFunction;
			compareFunction = _compareFunction;
		}
		quadProbHashTable(const quadProbHashTable& copySrc) : hashTable<TKey, TValue>((quadProbHashTable&)copySrc){
			hashFunction = copySrc.hashFunction;
			compareFunction = copySrc.compareFunction;
		}
		quadProbHashTable(quadProbHashTable&& moveSrc) : hashTable<TKey, TValue>((quadProbHashTable&& )moveSrc) {
			hashFunction = moveSrc.hashFunction;
			compareFunction = moveSrc.compareFunction;
		}

		double getNotExistItemCmpAvg() {
			size_t containerSize = this->getContainerSize();
			int cmpCount = 0;
			for (size_t k = 0; k < containerSize; k++) {
				size_t index = k;
				size_t newIndex = 0;
				for (size_t i = 0; i < containerSize; i++) {
					size_t step = i * i;
					newIndex = (index + step) % containerSize;
					if (this->isNeverused(newIndex)) {
						break;
					}
					cmpCount++;
				}
			}
			return (double)cmpCount / containerSize;
		}

		double getExistItemCmpAvg() {
			size_t containerSize = this->getContainerSize();
			int cmpCount = 0;
			for (size_t k = 0; k < containerSize; k++) {
				if (this->isNeverused(k))
					continue;
				const TKey& key = this->getContainerItem(k).key;
				size_t index = hashFunction(containerSize, key);
				size_t newIndex = 0;
				for (size_t i = 0; i < containerSize; i++) {
					size_t step = i * i;
					newIndex = (index + step) % containerSize;
					cmpCount++;
					if (!this->isNeverused(newIndex) && compareFunction(this->getContainerItem(newIndex).key, key) == 0) {
						break;
					}
				}
			}
			return (double)cmpCount / this->getCount();
		}

	protected:
		virtual size_t getPushLocation(const TKey& key) const {
			// a^2 = b^2 (mod c) if and only if a = b (mod c)
			size_t containerSize = this->getContainerSize();
			uint32_t hash = hashFunction(containerSize, key);
			size_t index = hash % containerSize;
			size_t newIndex = 0;
			for (size_t i = 0; i < containerSize; i++) {
				size_t step = i * i;
				newIndex = (index + step) % containerSize;
				if (this->isAvailable(newIndex))
					break;
			}
			if (!this->isAvailable(newIndex))
				throw runtime_error("there have no any suitable location to put.");
			return newIndex;
		}
		virtual size_t getItemLocation(const TKey& key) const {
			size_t containerSize = this->getContainerSize();
			uint32_t hash = hashFunction(containerSize, key);
			size_t index = hash % containerSize;
			size_t newIndex = 0;
			for (size_t i = 0; i < containerSize; i++) {
				size_t step = i * i;
				newIndex = (index + step) % containerSize;
				if (this->isOccupied(newIndex)) {
					if(compareFunction(this->getContainerItem(newIndex).key, key) == 0)
						return newIndex;
				}
				else if(this->isNeverused(newIndex)){
					break;
				}
			}
			return -1;
		}
	private:
		hashFunc<TKey> hashFunction;
		compareFunc<TKey> compareFunction;
	};

	template<typename TKey, typename TValue>
	class doubProbHashTable : public hashTable<TKey, TValue> {
	public:
		doubProbHashTable(size_t _tableSize, hashFunc<TKey> _hashFunction, compareFunc<TKey> _compareFunction, int _maxStep) : hashTable<TKey, TValue>(_tableSize){
			hashFunction = _hashFunction;
			compareFunction = _compareFunction;
			maxStep = _maxStep;
		}
		doubProbHashTable(const doubProbHashTable& copySrc) : hashTable<TKey, TValue>((doubProbHashTable&)copySrc) {
			hashFunction = copySrc.hashFunction;
			compareFunction = copySrc.compareFunction;
		}
		doubProbHashTable(doubProbHashTable&& moveSrc) : hashTable<TKey, TValue>((doubProbHashTable&&)moveSrc) {
			hashFunction = moveSrc.hashFunction;
			compareFunction = moveSrc.compareFunction;
		}

		double getExistItemCmpAvg() {
			size_t containerSize = this->getContainerSize();
			int cmpCount = 0;
			for (size_t k = 0; k < containerSize; k++) {
				if (this->isNeverused(k))
					continue;
				const TKey& key = this->getContainerItem(k).key;
				uint32_t hash = hashFunction(containerSize, key);
				size_t index = hash % containerSize;
				size_t newIndex = 0;
				size_t step = getStep(key);
				for (size_t i = 0; i < containerSize; i++) {
					newIndex = (index + step * i) % containerSize;
					cmpCount++;
					if (!this->isNeverused(newIndex) && compareFunction(this->getContainerItem(newIndex).key, key) == 0) {
						break;
					}
				}
			}
			return (double)cmpCount / this->getCount();
		}

	protected:
		virtual size_t getPushLocation(const TKey& key) const {
			// a^2 = b^2 (mod c) if and only if a = b (mod c)
			size_t containerSize = this->getContainerSize();
			uint32_t hash = hashFunction(containerSize, key);
			size_t step = getStep(key);
			size_t index = hash % containerSize;
			size_t newIndex = index;
			for (size_t i = 0; i < containerSize; i++) {
				if (this->isAvailable(newIndex))
					break;
				newIndex = (newIndex + step) % containerSize;
			}
			if (!this->isAvailable(newIndex))
				throw runtime_error("there have no any suitable location to put.");
			return newIndex;
		}
		virtual size_t getItemLocation(const TKey& key) const {
			size_t containerSize = this->getContainerSize();
			uint32_t hash = hashFunction(containerSize, key);
			size_t step = getStep(key);
			size_t index = hash % containerSize;
			size_t newIndex = index;
			for (size_t i = 0; i < containerSize; i++) {
				if (this->isOccupied(newIndex) && compareFunction(this->getContainerItem(newIndex).key, key) == 0) {
					return newIndex;
				}
				else if (this->isNeverused(newIndex)) {
					break;
				}
				newIndex = (newIndex + step) % containerSize;
			}
			return -1;
		}
	private:
		hashFunc<TKey> hashFunction;
		compareFunc<TKey> compareFunction;
		int maxStep;

		size_t getStep(const TKey key) const{
			uint32_t hash = hashFunction(maxStep, key);
			return maxStep - hash;
		}
	};
}

typedef struct {
	char id[10];
	char name[10];
	unsigned char score[6];
	float scoreAverage;
} studentInfo;
ostream& operator<<(ostream& outStream, const studentInfo& item) {
	outStream.write((char*)(&item), sizeof(item));
	return outStream;
}
istream& operator>>(istream& inStream, studentInfo& item) {
	inStream.read((char*)(&item), sizeof(item));
	return inStream;
}

typedef struct {
	int fileID;
	vector<studentInfo> studentInfos;
} studentStatFileInfo;

int stringCompare(const string& str1, const string& str2) {
	return str1.compare(str2);
}
uint32_t stringHash(const size_t hashTableSize, const string& str) {
	uint32_t output = 1;
	for (char ch : str)
		output = (output * ch) % hashTableSize;
	return output;
}

void printUsage() {
	printf("-----------------------------------------------------------------------------------\n");
	printf("Usage:");
	printf("\t1) Create Quadratic probing hash.\n");
	printf("\t2) Create double hash.\n");
	printf("\t0) Exit.\n");
	printf("\tIf you press Ctrl + Z while task ask you to input something, \n\tThis operation will stop current task.\n");
	printf("-----------------------------------------------------------------------------------\n");
}

studentStatFileInfo readStudentFile(bool& eofToken) {
	studentStatFileInfo output;
	int fileID;
	string fileName;
	bool isTextFile;
	bool isBinaryFile;
	bool success;
	do {
		printf("FileID: ");
		fileID = stdioUtility::readInt32(success, eofToken);
		fileName = "input" + stringUtility::toString(fileID);
		isTextFile = ioUtility::fileExists(fileName + ".txt");
		isBinaryFile = ioUtility::fileExists(fileName + ".bin");
		if (eofToken) {
			return output;
		}
		if (success && !isTextFile && !isBinaryFile) {
			printf("*** ERROR: File: %s.bin and %s.txt not found. ***\n", fileName.c_str(), fileName.c_str());
			success = false;
		}
	} while (!success);
	if (isTextFile && isBinaryFile) {
		int userSelected;
		do {
			printf("This file exists text format and binary format.\n");
			printf("Read: ");
			printf("\t[0]: Read text format.\n");
			printf("\t[1]: Read binary format.\n");
			printf("Select: ");
			userSelected = stdioUtility::readInt32(success, eofToken);
			if (eofToken) {
				return output;
			}
			if ((userSelected >> 1) != 0) {
				printf("*** ERROR: 0 or 1 excepted. ***\n");
				success = false;
			}
		} while (!success);
		if (userSelected == 0)
			isBinaryFile = false;
		else
			isTextFile = false;
	}
	if (isTextFile) {
		using namespace tableUtility;
		ofstream outStream;
		outStream.open(fileName + ".bin", ofstream::binary);
		table readTable;
		readTable.readFromFile(fileName + ".txt", '\t', false);
		for (int i = 0; i < readTable.getRowCount(); i++) {
			const tableRow& curRow = readTable[i];
			if (curRow.getColumnCount() != 9)
				continue;
			studentInfo stInfo = {};
			string stID, stName;
			curRow >> stID >> stName;
			stID.copy(stInfo.id, min(sizeof(stInfo.id), stID.length()), 0);
			stName.copy(stInfo.name, min(sizeof(stInfo.name), stName.length()), 0);
			for (int i = 0; i < 6; i++) {
				stInfo.score[i] = (unsigned char)curRow[i + 2].as_int_nochk();
			}
			stInfo.scoreAverage = curRow[8].asFloat_nochk();
			outStream << stInfo;
		}
		outStream.close();
		isTextFile = false;
		isBinaryFile = true;
	}
	ifstream inStream;
	inStream.open(fileName + ".bin", ifstream::binary);
	studentInfo tmpStInfo;
	while (inStream >> tmpStInfo) {
		output.studentInfos.push_back(tmpStInfo);
	}
	output.fileID = fileID;
	return output;
}

using namespace hashTableUtility;
using namespace mathUtility;

void Debug_testGetValue(const hashTable<string, studentInfo>& hashTb) {
	printf("!!!!Debug Zone!!!!\n");
	int containerSize = hashTb.getContainerSize();
	for (size_t i = 0; i < containerSize; i++) {
		if (hashTb.isAvailable(i))
			continue;
		keyValuePair<string, studentInfo> pair = hashTb.getContainerItem(i);
		printf("Testing: %s...\n", pair.key.c_str());
		if (!hashTb.contains(pair.key)) {
			printf("Failure: Contains method work abnormal.\n");
		}
		else {
			studentInfo stInfo = hashTb.getValue(pair.key);
			printf("RetData: %10s%10s %3.3f\n", pair.value.id, pair.value.name, pair.value.scoreAverage);
		}
		printf("End testing: %s...\n\n", pair.key.c_str());
	}
	printf("^^^^Debug Zone^^^^\n");
}

void Task1() {
	bool eofToken;
	studentStatFileInfo stStatFileInfo = readStudentFile(eofToken);
	if (eofToken) {
		printf("User stop the operation.\n");
		return;
	}
	primeTable primeTb(stStatFileInfo.studentInfos.size() << 1);
	int hashTbSize = primeTb.getNearbyMax((int)((stStatFileInfo.studentInfos.size()) * 1.2));
	quadProbHashTable<string, studentInfo> quadProbTb(hashTbSize, stringHash, stringCompare);
	for (const studentInfo& stInfo : stStatFileInfo.studentInfos) {
		quadProbTb.add(string(stInfo.id), stInfo);
	}
	ofstream outStream;
	outStream.open("quadratic" + stringUtility::toString(stStatFileInfo.fileID) + ".txt");
	outStream << " --- Hash table created by Quadratic probing ---\n";
	for (size_t i = 0; i < hashTbSize; i++) {
		if (quadProbTb.isOccupied(i)) {
			const keyValuePair<string, studentInfo>& pair = quadProbTb.getContainerItem(i);
			outStream <<
				stringUtility::formatString("[%3d] %10d, %10s, %10s, ",
					i, stringHash(hashTbSize, pair.key), pair.value.id, pair.value.name)
				<< stringUtility::toString(pair.value.scoreAverage, 10) << '\n';
		}
		else {
			outStream << stringUtility::formatString("[%3d] ", i) << '\n';
		}
	}
	outStream << " ----------------------------------------------------- \n";
	outStream.close();
	printf("Unsuccessful search: %3.4f\n", quadProbTb.getNotExistItemCmpAvg());
	printf("Successful search: %3.4f\n", quadProbTb.getExistItemCmpAvg());
	//Debug_testGetValue(quadProbTb);
}
void Task2() {
	bool eofToken;
	studentStatFileInfo stStatFileInfo = readStudentFile(eofToken);
	if (eofToken) {
		printf("User stop the operation.\n");
		return;
	}
	primeTable primeTb(stStatFileInfo.studentInfos.size() << 1);
	int hashTbSize = primeTb.getNearbyMax((int)((stStatFileInfo.studentInfos.size()) * 1.2));
	doubProbHashTable<string, studentInfo> dbProbTb(hashTbSize, stringHash, stringCompare, primeTb.getNearbyMax((int)ceil(stStatFileInfo.studentInfos.size() / 3.0)));;
	for (const studentInfo& stInfo : stStatFileInfo.studentInfos) {
		dbProbTb.add(string(stInfo.id), stInfo);
	}
	ofstream outStream;
	outStream.open("double" + stringUtility::toString(stStatFileInfo.fileID) + ".txt");
	outStream << " --- Hash table created by Double hashing ---\n";
	for (size_t i = 0; i < hashTbSize; i++) {
		if (dbProbTb.isOccupied(i)) {
			const keyValuePair<string, studentInfo>& pair = dbProbTb.getContainerItem(i);
			outStream <<
				stringUtility::formatString("[%3d] %10d, %10s, %10s, ",
					i, stringHash(hashTbSize, pair.key), pair.value.id, pair.value.name)
				<< stringUtility::toString(pair.value.scoreAverage, 10) << '\n';
		}
		else {
			outStream << stringUtility::formatString("[%3d] ", i) << '\n';
		}
	}
	outStream << " ----------------------------------------------------- \n";
	outStream.close();
	printf("Successful search: %3.4f\n", dbProbTb.getExistItemCmpAvg());
	//Debug_testGetValue(dbProbTb);
}
void Task3() {
}
int main() {
	bool success = false;
	bool exitFlag = false;
	bool eofToken = false;
	while (!exitFlag) {
		int selectTask = -1;
		do {
			printUsage();
			printf("Select Task: ");
			selectTask = stdioUtility::readInt32(success, eofToken);
			if (eofToken) {
				exitFlag = true;
				break;
			}
		} while (!success);
		switch (selectTask) {
		case 0:
			exitFlag = true;
			break;
		case 1:
			try {
				printf(">>>>>>>>>>>>  Task 1 Start  <<<<<<<<<<<<\n");
				Task1();
				printf("<<<<<<<<<<<<  Task 1 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("!!!!!!!!!!!!  Task 1 Abort  !!!!!!!!!!!!\n\n");
			}
			break;
		case 2:
			try {
				printf(">>>>>>>>>>>>  Task 2 Start  <<<<<<<<<<<<\n");
				Task2();
				printf("<<<<<<<<<<<<  Task 2 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("!!!!!!!!!!!!  Task 2 Abort  !!!!!!!!!!!!\n\n");
			}
			break;
		case 3:
			try {
				printf(">>>>>>>>>>>>  Task 3 Start  <<<<<<<<<<<<\n");
				Task3();
				printf("<<<<<<<<<<<<  Task 3 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("!!!!!!!!!!!!  Task 3 Abort  !!!!!!!!!!!!\n\n");
			}
			break;
		default:
			printf("*** ERROR: Unknown task: %d ***\n", selectTask);
			break;
		}
	}
	return 0;
}
