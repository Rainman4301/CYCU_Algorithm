/*
	===================================
	             Group: 30
	=================+=================
	    ID           |    Name         
	-----------------+-----------------
	   10722217      |   廖宇傑         
	-----------------+-----------------
	   10911101      |   鄭皓元
	=================+=================
	   Encoding:  UTF8
	   Compiler:  Visual C++ v143
	===================================
*/
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <iomanip>
#include <algorithm>
#include <iterator>
#include <random>
#include <thread>

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
					if (!isReadDot)
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
			return formatString("%*d.%*d", totalWidth - precision - 1, aPart, precision, bPart);
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
	float readFloat(bool& success, bool& eofToken) {
		string readStr = readString(success, eofToken);
		if (success && !eofToken) {
			double readDouble = stringUtility::tryParseToDouble(readStr, success);
			return readDouble;
		}
		return NAN;
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

		float readFloat() {
			float outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}
		double readDouble() {
			double outputData;
			size_t readLength = readChars((char*)(&outputData), sizeof(outputData));
			if (readLength != sizeof(outputData))
				throw runtime_error("");
			return outputData;
		}

		size_t getReadableLength() {
			return getPosition() - getLength();
		}

		virtual size_t readChars(char* target, size_t length) { return -1; }
		virtual size_t getPosition() { return -1; }
		virtual size_t getLength() { return -1; }

		virtual bool isEnd() { return true; }
	};
	class memoryBinaryReader : public binaryReader {
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
		streamBinaryReader(istream& inputStream) :baseStream(inputStream) {
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

		bool isEnd()
		{
			return baseStream.peek() < 0 || baseStream.eof();
		}

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
		void writeDouble(double value) {
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






//Classes that provides the implementation of studentGraph.
namespace studentGraph {
	class digraph;
	class vertexInfo;
	class adjListNode;
	class adjListNodeComparer;

	class adjListNode {
	public:
		vertexInfo* pVertex;
		float weight;

		adjListNode();
		adjListNode(vertexInfo* _pVertex, float _weight);

		bool operator==(const adjListNode& node);
	};
	class adjListNodeComparer {
	public:
		bool operator()(const adjListNode& nodeA, const adjListNode& nodeB) const;
	};
	class vertexInfoPtrComparer {
	public:
		bool operator()(vertexInfo* const nodeA, vertexInfo* const nodeB) const;
	};
	class vertexInfo {
		friend class digraph;
	public:
		string stID;
		set<adjListNode, adjListNodeComparer> adjList;

		vertexInfo();
		vertexInfo(const string& _stID);

		void addAdjNode(vertexInfo* adjVertex, float weight);
	private:
		void* operator new(size_t size);
		void operator delete(void* delPtr);
		void operator delete[](void* delPtr);
	};
	class digraph {
	public:
		// Add a vertex into this digraph if this vertex not exists.
		void addVertex(const string& stID);
		void addEdge(const string& senderStID, const string& receiverStID, float weight);

		int getConnCount(vertexInfo* const senderVertex, unordered_set<vertexInfo*>& output, unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer);
		int getVerticesCount() const;
		int getTotalNodeCount() const;

		void writeToFile(const string& fileName);
		void writeConnResultToFile(const string& fileName);
		void writeInfluenceResultToFile(float threshold, const string& fileName);
		void writeProbInfluenceResultToFile(const string& fileName);

		void clearAll();

		~digraph();
	private:
		map<string, vertexInfo*> vertices;

		void calculateInfluence(vertexInfo* vertex,float thresholdVal, int level, unordered_set<vertexInfo*>& visitedVertex, unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer);
		void calculateProbInfluence(vertexInfo* vertex, random_device& randDev, int level, unordered_set<vertexInfo*>& visitedVertex, unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer);
		
	};
	// Method implements

	void digraph::addVertex(const string& stID) {
		if (vertices.find(stID) == vertices.end())
			vertices[stID] = new vertexInfo(string(stID));
	}
	void digraph::addEdge(const string& senderStID, const string& receiverStID, float weight) {
		addVertex(senderStID);
		addVertex(receiverStID);
		vertexInfo* senderVertex = vertices[senderStID];
		vertexInfo* receiverVertex = vertices[receiverStID];
		senderVertex->addAdjNode(receiverVertex, weight);
	}

	void digraph::writeToFile(const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);
		ofs << "<<< There are " << vertices.size() << " IDs in total. >>>\n";
		int index = 1, totalNodes = 0;
		for (auto v : vertices) {
			int innerIndex = 1;
			if (index > 1)
				ofs << '\n';
			ofs << "[" << setw(3) << index << "] " << v.first << ": \n";
			vertexInfo* vertexInf = v.second;
			for (auto adj : vertexInf->adjList) {
				ofs << "\t";
				ofs << "(" << setw(2) << innerIndex << ") " << adj.pVertex->stID << ",  " << setw(4) << setprecision(3) << adj.weight;
				if (innerIndex > 0 && ((innerIndex % 10) == 0))
					ofs << "\n";
				innerIndex++;
				totalNodes++;
			}
			index++;
		}
		ofs << "\n<<< There are " << totalNodes << " nodes in total. >>>\n";
		ofs.close();
	}

	void digraph::writeConnResultToFile(const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);
		ofs << "<<< There are " << vertices.size() << " IDs in total. >>>\n";
		int index = 1, verticesCount = vertices.size();
		unordered_map<vertexInfo*, unordered_set<vertexInfo*>> output;
		vector <pair<int, vertexInfo*>> connCounter;
		for (const auto& v : vertices) {
			int innerIndex = 1;
			vertexInfo* vertexInf = v.second;
			output[v.second] = unordered_set<vertexInfo*>();
			unordered_set<vertexInfo*>& connVertices = output[v.second];
			int connCount = getConnCount(vertexInf, connVertices, output);
			int connCountHash = (verticesCount - connCount) * verticesCount + index;
			connCounter.push_back(pair<int, vertexInfo*>(connCountHash, v.second));
			index++;
		}
		index = 1;
		sort(connCounter.begin(), connCounter.end());
		for (const auto& connCountInf : connCounter) {
			unordered_set<vertexInfo*>& connVerticesSet = output[connCountInf.second];
			vector<vertexInfo*> connVertices;
			for (auto& v : connVerticesSet) {
				if (v != connCountInf.second)
					connVertices.push_back(v);
			}
			sort(connVertices.begin(), connVertices.end(), vertexInfoPtrComparer());
			int innerIndex = 1;
			int connCount = connVertices.size();
			if (index > 1)
				ofs << '\n';
			ofs << "[" << setw(3) << index << "] " << connCountInf.second->stID << "(" << connCount << "): \n";
			for (const auto& connVertex : connVertices) {
				ofs << '\t';
				ofs << '(' << setw(2) << innerIndex << ") " << connVertex->stID;
				if (innerIndex > 0 && ((innerIndex % 10) == 0))
					ofs << '\n';
				innerIndex++;
			}
			index++;
		}
		ofs << '\n';
		ofs.close();
	}
	void digraph::writeInfluenceResultToFile(float threshold, const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);
		int index = 1, verticesCount = vertices.size();
		unordered_map<vertexInfo*, unordered_set<vertexInfo*>> output;
		vector<pair<int, vertexInfo*>> influenceCountTb;
		for (auto& vertex : vertices) {
			unordered_set<vertexInfo*>& visitedVertex = output[vertex.second];
			calculateInfluence(vertex.second, threshold, 0, visitedVertex, output);
			int wei = (verticesCount - visitedVertex.size()) * verticesCount + index;
			if(visitedVertex.size() > 1)
				influenceCountTb.push_back(pair<int, vertexInfo*>(wei, vertex.second));
			index++;
		}
		sort(influenceCountTb.begin(), influenceCountTb.end());
		index = 1;
		ofs << "<<< There are " << influenceCountTb.size() << " IDs in total. >>>\n";
		for (const auto& influenceCountInf : influenceCountTb) {
			unordered_set<vertexInfo*>& visitedVertex = output[influenceCountInf.second];
			vector<vertexInfo*> tmp;
			for (auto& v : visitedVertex) {
				if (v != influenceCountInf.second) {
					tmp.push_back(v);
				}
			}
			sort(tmp.begin(), tmp.end(), vertexInfoPtrComparer());
			int innerIndex = 1;
			int inffCount = visitedVertex.size() - 1;
			if (index > 1)
				ofs << '\n';
			ofs << "[" << setw(3) << index << "] " << influenceCountInf.second->stID << "(" << inffCount << "): \n";
			for (const auto& inffVertex : tmp) {
				ofs << '\t';
				ofs << '(' << setw(2) << innerIndex << ") " << inffVertex->stID;
				if (innerIndex > 0 && ((innerIndex % 10) == 0))
					ofs << '\n';
				innerIndex++;
			}
			index++;
		}
		ofs << '\n';
		ofs.close();
		//calculateInfluence
	}
	void digraph::writeProbInfluenceResultToFile(const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);
		int index = 1, verticesCount = vertices.size();
		unordered_map<vertexInfo*, unordered_set<vertexInfo*>> output;
		vector<pair<int, vertexInfo*>> influenceCountTb;
		random_device randDev;
		for (auto& vertex : vertices) {
			unordered_set<vertexInfo*>& visitedVertex = output[vertex.second];
			calculateProbInfluence(vertex.second, randDev, 0, visitedVertex, output);
			int wei = (verticesCount - visitedVertex.size()) * verticesCount + index;
			if (visitedVertex.size() > 1)
				influenceCountTb.push_back(pair<int, vertexInfo*>(wei, vertex.second));
			index++;
		}
		sort(influenceCountTb.begin(), influenceCountTb.end());
		index = 1;
		ofs << "<<< There are " << influenceCountTb.size() << " IDs in total. >>>\n";
		for (const auto& influenceCountInf : influenceCountTb) {
			unordered_set<vertexInfo*>& visitedVertex = output[influenceCountInf.second];
			vector<vertexInfo*> tmp;
			for (auto& v : visitedVertex) {
				if (v != influenceCountInf.second) {
					tmp.push_back(v);
				}
			}
			sort(tmp.begin(), tmp.end(), vertexInfoPtrComparer());
			int innerIndex = 1;
			int inffCount = visitedVertex.size() - 1;
			if (index > 1)
				ofs << '\n';
			ofs << "[" << setw(3) << index << "] " << influenceCountInf.second->stID << "(" << inffCount << "): \n";
			for (const auto& inffVertex : tmp) {
				ofs << '\t';
				ofs << '(' << setw(2) << innerIndex << ") " << inffVertex->stID;
				if (innerIndex > 0 && ((innerIndex % 10) == 0))
					ofs << '\n';
				innerIndex++;
			}
			index++;
		}
		ofs << '\n';
		ofs.close();
		//calculateInfluence
	}

	int digraph::getConnCount(vertexInfo* const senderVertex, unordered_set<vertexInfo*>& visitedVertex, unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer) {
		queue<vertexInfo*> visitQue;
		visitedVertex.insert(senderVertex);
		for (const auto& adj : senderVertex->adjList) {
			visitQue.push(adj.pVertex);
			visitedVertex.insert(adj.pVertex);
		}
		while (!visitQue.empty() && (visitedVertex.size() < vertices.size())) {
			vertexInfo* curVertex = visitQue.front();
			visitQue.pop();
			const set<adjListNode, adjListNodeComparer>& adjList = curVertex->adjList;
			for (const auto& adj : adjList) {
				if (visitedVertex.find(adj.pVertex) != visitedVertex.end())
					continue;
				unordered_set<vertexInfo*>& calculatedConnVertexSet = buffer[adj.pVertex];
				if (!calculatedConnVertexSet.empty()) {
					for (auto& v : calculatedConnVertexSet) {
						if (visitedVertex.find(v) == visitedVertex.end())
							visitedVertex.insert(v);
					}
				}
				else {
					visitQue.push(adj.pVertex);
					visitedVertex.insert(adj.pVertex);
				}
			}
		}
		return visitedVertex.size() - 1;
	}
	int digraph::getVerticesCount() const {
		return vertices.size();
	}
	int digraph::getTotalNodeCount() const {
		int totalNodeCount = 0;
		for (const auto& v : vertices)
			totalNodeCount += v.second->adjList.size();
		return totalNodeCount;
	}

	void studentGraph::digraph::clearAll() {
		for (auto v : vertices)
			if (v.second != NULL)
				delete v.second;
		vertices.clear();
	}

	digraph::~digraph() {
		for (auto v : vertices)
			if (v.second != NULL)
				delete v.second;
	}

	void digraph::calculateInfluence(vertexInfo* vertex,
	 								float thresholdVal, int level,
	  								unordered_set<vertexInfo*>& visitedVertex,
	   								unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer)



	{
		auto& adjList = vertex->adjList;
		visitedVertex.insert(vertex);
		for (auto& adjNode : adjList) {
			if (adjNode.weight >= thresholdVal) {
				if (visitedVertex.find(adjNode.pVertex) == visitedVertex.end()) {
					auto& calculatedResult = buffer[adjNode.pVertex];
					visitedVertex.insert(adjNode.pVertex);
					if (!calculatedResult.empty()) {
						for (auto& pVertex : calculatedResult)
							if (visitedVertex.find(pVertex) == visitedVertex.end())
								visitedVertex.insert(pVertex);
					}
					else {
						calculateInfluence(adjNode.pVertex, thresholdVal, level + 1, visitedVertex, buffer);
					}
				}
			}
		}
	}

	

	void digraph::calculateProbInfluence(vertexInfo* vertex, random_device& randDev, int level, unordered_set<vertexInfo*>& visitedVertex, unordered_map<vertexInfo*, unordered_set<vertexInfo*>>& buffer){
		auto& adjList = vertex->adjList;
		visitedVertex.insert(vertex);
		uniform_real_distribution<float> distr(0.0f, 1.0f);
		for (auto& adjNode : adjList) {
			float thresholdVal = distr(randDev);
			if (adjNode.weight >= thresholdVal) {
				if (visitedVertex.find(adjNode.pVertex) == visitedVertex.end()) {
					auto& calculatedResult = buffer[adjNode.pVertex];
					visitedVertex.insert(adjNode.pVertex);
					if (!calculatedResult.empty()) {
						for (auto& pVertex : calculatedResult)
							if (visitedVertex.find(pVertex) == visitedVertex.end())
								visitedVertex.insert(pVertex);
					}
					else {
						calculateProbInfluence(adjNode.pVertex, randDev, level + 1, visitedVertex, buffer);
					}
				}
			}
		}
	}

	vertexInfo::vertexInfo() {
		stID = string();
	}
	vertexInfo::vertexInfo(const string& _stID) {
		stID = _stID;
	}
	void vertexInfo::addAdjNode(vertexInfo* adjVertex, float weight) {
		adjListNode newNode(adjVertex, weight);
		if (adjList.find(newNode) == adjList.end())
			adjList.insert(newNode);
	}
	void* vertexInfo::operator new(size_t size) {
		return ::operator new(size);
	}
	void vertexInfo::operator delete(void* delPtr) {
		::operator delete(delPtr);
	}
	void vertexInfo::operator delete[](void* delPtr) {
		::operator delete[](delPtr);
	}

	adjListNode::adjListNode() {
		pVertex = NULL;
		weight = 0;
	}
	adjListNode::adjListNode(vertexInfo* _pVertex, float _weight) {
		pVertex = _pVertex;
		weight = _weight;
	}
	bool adjListNode::operator==(const adjListNode& node) {
		return pVertex == node.pVertex && pVertex != NULL;
	}
	
	bool adjListNodeComparer::operator()(const adjListNode& nodeA, const adjListNode& nodeB) const {
		if (nodeA.pVertex == NULL || nodeB.pVertex == NULL)
			return false;
		return nodeA.pVertex->stID.compare(nodeB.pVertex->stID) < 0;
	}

	bool studentGraph::vertexInfoPtrComparer::operator()(vertexInfo* const nodeA, vertexInfo* const nodeB) const
	{
		return nodeA->stID.compare(nodeB->stID) < 0;
	}


}

class programStatus {
public:
	studentGraph::digraph currentDigraph;
	bool haveOpenFile;
	string fileName;
	int fileID;

	programStatus() : haveOpenFile(false), fileID(-1) {

	}

	void reset() {
		currentDigraph.clearAll();
		haveOpenFile = false;
		fileName = "";
		fileID = -1;
	}
};

programStatus _programStatus;

void printUsage() {
	printf("===================================================================================\n");
	printf("Usage:");
	printf("\t1) Create Direction Graph.\n");
	if (_programStatus.haveOpenFile)
		printf("\t2) Calculate connection count.\n");
	else
		printf("\t\033[31m2) Calculate connection count.\033[0m\n");
	if (_programStatus.haveOpenFile)
		printf("\t3) Calculate inflence count.\n");
	else
		printf("\t\033[31m3) Calculate inflence count.\033[0m\n");
	if (_programStatus.haveOpenFile)
		printf("\t4) Calculate probability inflence count.\n");
	else
		printf("\t\033[31m4) Calculate probability inflence count.\033[0m\n");
	printf("\t0) Exit.\n");
	printf("\tIf you press Ctrl + Z while task ask you to input something, \n\tThis operation will stop current task.\n");
	printf("-----------------------------------------------------------------------------------\n");
	if (_programStatus.haveOpenFile)
		printf("\tCurrent opened file: %s\n", _programStatus.fileName.c_str());
	else
		printf("\tCurrent opened file: (none)\n");
	printf("===================================================================================\n");
}

void getReadFileID(const string& message, const string& filenamePrefix, const string& filenameExt, int& outFileID, string& outFileName, bool& eofToken) {
	using namespace ioUtility;
	using namespace stdioUtility;

	int fileID;
	string fileName;
	bool success;

	do {
		printf("%s:", message.c_str());
		fileID = readInt32(success, eofToken);
		if (eofToken)
			return;
		else if (success) {
			fileName = filenamePrefix + stringUtility::toString(fileID) + filenameExt;
			if (!fileExists(fileName)) {
				printf("*** ERROR: File: %s not found. ***\n", fileName.c_str());
				success = false;
			}
		}
	} while (!success);
	outFileID = fileID;
	outFileName = fileName;
}

void Task1() {
	using namespace ioUtility;
	using namespace studentGraph;

	bool eofToken;
	int fileID;
	string fileName;

	getReadFileID("Enter File ID", "pairs", ".bin", fileID, fileName, eofToken);
	if (eofToken) {
		printf("You canceled this operation.\n");
		return;
	}
	_programStatus.reset();
	_programStatus.fileID = fileID;
	_programStatus.fileName = fileName;
	_programStatus.haveOpenFile = true;

	ifstream ifs;
	ifs.open(_programStatus.fileName, ifstream::binary);
	streamBinaryReader binReader(ifs);

	char senderIDBuf[10], receiverIDBuf[10];
	float weightBuf;
	while (!binReader.isEnd()) {
		binReader.readChars(senderIDBuf, sizeof(senderIDBuf));
		binReader.readChars(receiverIDBuf, sizeof(receiverIDBuf));
		weightBuf = binReader.readFloat();
		_programStatus.currentDigraph.addEdge(senderIDBuf, receiverIDBuf, weightBuf);
	}
	ifs.close();
	_programStatus.currentDigraph.writeToFile("pairs" + stringUtility::toString(_programStatus.fileID) + ".adj");
	printf("Number of vertices: %d\n", _programStatus.currentDigraph.getVerticesCount());
	printf("Number of nodes: %d\n", _programStatus.currentDigraph.getTotalNodeCount());
}

void Task2() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create direction graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		printf("Number of vertices: %d\n", _programStatus.currentDigraph.getVerticesCount());
		timerUtility::timer timer;
		timer.start();
		_programStatus.currentDigraph.writeConnResultToFile("pairs" + stringUtility::toString(_programStatus.fileID) + ".cnt");
		timer.stop();
		printf("Duration: %.3lfms\n", timer.getDuration());
	}
}

void Task3() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create direction graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		float inputThreshold;
		bool eofToken, success;
		do {
			printf("Enter threshold value: ");
			inputThreshold = stdioUtility::readFloat(success, eofToken);
			if (eofToken) {
				printf("You canceled this operation.\n");
				return;
			}
			if (success && inputThreshold < 0 || inputThreshold > 1) {
				printf("Threshold value should be in [0,1].\n");
				success = false;
			}
		} while (!success);
		printf("Number of vertices: %d\n", _programStatus.currentDigraph.getVerticesCount());
		timerUtility::timer timer;
		timer.start();
		_programStatus.currentDigraph.writeInfluenceResultToFile(inputThreshold, "pairs" + stringUtility::toString(_programStatus.fileID) + ".inf");
		timer.stop();
		printf("Duration: %.3lfms\n", timer.getDuration());
	}
}

void Task4() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create direction graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		printf("Number of vertices: %d\n", _programStatus.currentDigraph.getVerticesCount());
		timerUtility::timer timer;
		timer.start();
		_programStatus.currentDigraph.writeProbInfluenceResultToFile("pairs" + stringUtility::toString(_programStatus.fileID) + ".pro");
		timer.stop();
		printf("Duration: %.3lfms\n", timer.getDuration());
	}
}

void Debug_ExportAsDotFile() {
	using namespace ioUtility;
	using namespace studentGraph;

	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create direction graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		ifstream ifs;
		ofstream ofs;
		ifs.open(_programStatus.fileName, ifstream::binary);
		ofs.open(stringUtility::formatString("digraph%d.dot", _programStatus.fileID));
		streamBinaryReader binReader(ifs);

		char senderIDBuf[10], receiverIDBuf[10];
		float weightBuf;
		ofs << "digraph " << _programStatus.fileID << "{\n";
		while (!binReader.isEnd()) {
			binReader.readChars(senderIDBuf, sizeof(senderIDBuf));
			binReader.readChars(receiverIDBuf, sizeof(receiverIDBuf));
			weightBuf = binReader.readFloat();
			ofs << stringUtility::formatString("\t%s -> %s\n", senderIDBuf, receiverIDBuf);
		}
		ofs << "}\n";
		ifs.close();
		ofs.close();
	}
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
				printf("\033[31m!!!!!!!!!!!!  Task 1 Abort  !!!!!!!!!!!!\033[0m\n\n");
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
				printf("\033[31m!!!!!!!!!!!!  Task 2 Abort  !!!!!!!!!!!!\033[0m\n\n");
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
				printf("\033[31m!!!!!!!!!!!!  Task 3 Abort  !!!!!!!!!!!!\033[0m\n\n");
			}
			break;
		case 4:
			try {
				printf(">>>>>>>>>>>>  Task 4 Start  <<<<<<<<<<<<\n");
				Task4();
				printf("<<<<<<<<<<<<  Task 4 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("\033[31m!!!!!!!!!!!!  Task4 Abort  !!!!!!!!!!!!\033[0m\n\n");
			}
			break;
		case 9:
			try {
				printf(">>>>>>>>>>>>  Debug Start  <<<<<<<<<<<<\n");
				Debug_ExportAsDotFile();
				printf("<<<<<<<<<<<<  Debug End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("\033[31m!!!!!!!!!!!!  Task 3 Abort  !!!!!!!!!!!!\033[0m\n\n");
			}
			break;
		default:
			printf("*** ERROR: Unknown task: %d ***\n", selectTask);
			break;
		}
	}
	return 0;
}
