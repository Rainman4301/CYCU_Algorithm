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
#include <limits>

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
	class graph;
	class vertexInfo;
	class adjListNode;



	// 相鄰串列內儲存之節點
	class adjListNode {
	public:
		vertexInfo* pVertex;
		float weight;

		adjListNode();
		adjListNode(vertexInfo* _pVertex, float _weight);

		bool operator==(const adjListNode& node) const;

		class comparer {
		public:
			bool operator()(const adjListNode& nodeA, const adjListNode& nodeB) const;
		};

		class hasher {
		public:
			size_t operator()(const adjListNode& nodeA) const;
		};
	};


	class vertexInfoPtrComparer {
	public:
		bool operator()(vertexInfo* const nodeA, vertexInfo* const nodeB) const;
	};



	class vertexInfo {
		friend class graph;
	public:
		using adjListIterator = unordered_set<adjListNode, adjListNode::hasher>::iterator;

		string stID;
		unordered_set<adjListNode, adjListNode::hasher> adjList;

		vertexInfo();
		vertexInfo(const string& _stID);

		void addAdjNode(vertexInfo* adjVertex, float weight);
	private:
		void* operator new(size_t size);
		void operator delete(void* delPtr);
		void operator delete[](void* delPtr);
	};


	class graph {
	public:
		// Add a vertex into this graph if this vertex not exists.
		void addVertex(const string& stID);
		void addEdge(const string& senderStID, const string& receiverStID, float weight);

		bool containsVertex(const string& stID) const;
		bool containsEdge(const string& stID1, const string& stID2) const;

		int getVerticesCount() const;
		int getTotalNodeCount() const;
		void getStudents(vector<string>& output) const;

		void writeToFile(const string& fileName);
		void writeConnectComponentToFile(const string& fileName);
		void writeShortpathToFile(const string& stID, const string& fileName);

		void outMinSpinningTree();

		void clearAll();

		~graph();
	private:
		unordered_map<string, vertexInfo*> vertices;
		// 每個頂點資訊指標對應到一個用來儲存頂點有連通的頂點指標。
		unordered_map<vertexInfo*, unordered_set<vertexInfo*>*> connectedComponentMap;
		vector<unordered_set<vertexInfo*>*> connectedComponents;
		unordered_map<vertexInfo*, unordered_map<vertexInfo*, float>> shortpathWeights;
		unordered_map<vertexInfo*, unordered_map<vertexInfo*, float>> longestpathWeights;
		unordered_map< unordered_set<vertexInfo*>*, double> minSpinningTreeSumMap;

		void calculateConnectedComponents(vertexInfo* ptrCalculateVertex);
		void findShortestPath(vertexInfo* ptrStartVertex);
		void findLongestPath(vertexInfo* ptrStartVertex);
		void finsMinSpinningTree(vertexInfo* ptrStartVertex);
	};



	class connectComponentOutputComparer {
	public:
		bool operator()(const vector<vertexInfo*>& nodeA, const vector<vertexInfo*>& nodeB) const;
	};


	class minSpinnOutputComparer {
	public:
		bool operator()(const pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>& nodeA, const pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>& nodeB) const;
	};



	class shortestPathOutputComparer {
	public:
		bool operator()(const pair<float, vertexInfo*>& item1, pair<float, vertexInfo*>& item2);
	};



	using edge = pair<vertexInfo*, vertexInfo*>;

	// Method implements

	void graph::addVertex(const string& stID) {
		if (vertices.find(stID) == vertices.end()) {
			vertexInfo* ptrVertex = vertices[stID] = new vertexInfo(string(stID));
			connectedComponentMap[ptrVertex] = NULL;
		}
	}

	void graph::addEdge(const string& senderStID, const string& receiverStID, float weight) {
		addVertex(senderStID);
		addVertex(receiverStID);
		vertexInfo* senderVertex = vertices[senderStID];
		vertexInfo* receiverVertex = vertices[receiverStID];
		senderVertex->addAdjNode(receiverVertex, weight);
		receiverVertex->addAdjNode(senderVertex, weight);
	}

	bool graph::containsVertex(const string& stID) const {
		return vertices.find(stID) != vertices.end();
	}

	bool graph::containsEdge(const string& stID1, const string& stID2) const {
		return containsVertex(stID1) && containsVertex(stID2) && vertices.at(stID1)->adjList.find(adjListNode(vertices.at(stID2), 0)) != vertices.at(stID1)->adjList.end();
	}

	void graph::writeToFile(const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);
		ofs << "<<< There are " << vertices.size() << " IDs in total. >>>\n";
		vector<vertexInfo*> sortedVertices;
		for (auto& v : vertices)
			sortedVertices.push_back(v.second);
		sort(sortedVertices.begin(), sortedVertices.end(), vertexInfoPtrComparer());
		int index = 1, totalNodes = 0;
		for (auto v : sortedVertices) {
			int innerIndex = 1;
			if (index > 1)
				ofs << '\n';
			ofs << "[" << setw(3) << index << "] " << v->stID << ": \n";
			vertexInfo* vertexInf = v;
			vector<adjListNode> sortedAdjListNode;
			for (auto adj : vertexInf->adjList)
				sortedAdjListNode.push_back(adj);
			sort(sortedAdjListNode.begin(), sortedAdjListNode.end(), adjListNode::comparer());
			for (auto& adjNode : sortedAdjListNode) {
				ofs << "\t";
				ofs << "(" << setw(2) << innerIndex << ") " << adjNode.pVertex->stID << ",  " << setw(4) << setprecision(3) << adjNode.weight;
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

	void graph::writeConnectComponentToFile(const string& fileName) {
		ofstream ofs;
		ofs.open(fileName);

		vector<vector<vertexInfo*>> outputs;
		unordered_set<unordered_set<vertexInfo*>*> outputedCC;
		for (auto& v : vertices) {
			if (connectedComponentMap[v.second] == NULL) {
				calculateConnectedComponents(v.second);
			}
			unordered_set<vertexInfo*>* ptrConnectedComponent = connectedComponentMap[v.second];
			if (outputedCC.find(ptrConnectedComponent) == outputedCC.end()) {
				unordered_set<vertexInfo*>& connectedComponent = *ptrConnectedComponent;
				outputs.push_back(vector<vertexInfo*>());
				vector<vertexInfo*>& sortedResult = outputs.back();
				for (auto& connV : connectedComponent) {
					sortedResult.push_back(connV);
				}
				sort(sortedResult.begin(), sortedResult.end(), vertexInfoPtrComparer());
				outputedCC.insert(&connectedComponent);
			}

		}
		sort(outputs.begin(), outputs.end(), connectComponentOutputComparer());

		ofs << "<<< There are " << outputs.size() << " connected components in total. >>>\n";
		int counter = 1;
		for (auto& output : outputs) {
			ofs << "{" << setw(2) << counter << "} Connected Component: size = " << output.size();
			printf("Connected Component {%2d}: %3d\n", counter, output.size());
			int innerCounter = 0;
			for (auto& v : output) {
				if ((innerCounter & 7) == 0)
					ofs << '\n';
				ofs << "\t(" << setw(3) << innerCounter + 1 << ") " << v->stID << " ";
				innerCounter++;
			}
			ofs << '\n';
			counter++;
		}
		ofs.close();
	}

	void graph::writeShortpathToFile(const string& stID, const string& fileName) {
		ofstream ofs;
		ofs.open(fileName, fstream::app);
		if (containsVertex(stID))
			findShortestPath(vertices[stID]);
		vertexInfo* vertex = vertices[stID];
		ofs << "\norigin:" << setw(9) << stID << '\n';
		unordered_map<vertexInfo*, float>& weight = shortpathWeights[vertex];
		vector<pair<float, vertexInfo*>> sortedWeight;
		for (auto& weightData : weight) {
			if (weightData.first != vertex)
				sortedWeight.push_back(pair<float, vertexInfo*>(weightData.second, weightData.first));
		}
		sort(sortedWeight.begin(), sortedWeight.end(), shortestPathOutputComparer());
		int index = 0;
		for (auto& outputData : sortedWeight) {
			if (((index & 7) == 0) && index > 0)
				ofs << '\n';
			ofs << '(' << setw(2) << index + 1 << ") \t" << outputData.second->stID << ", " << setprecision(3) << outputData.first << '\t';
			index++;
		}
		ofs << "\n\n";
		ofs.close();
	}


	void graph::outMinSpinningTree() {
		for (auto& vertex : vertices) {
			finsMinSpinningTree(vertex.second);
		}
		vector<pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>> outputs;

		for (auto& cc : connectedComponents) {
			unordered_set<vertexInfo*>* ptrConnectedComponent = cc;
			unordered_set<vertexInfo*>& connectedComponent = *ptrConnectedComponent;
			outputs.push_back(pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>());
			pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>& sortedResult = outputs.back();
			outputs.back().second = ptrConnectedComponent;
			for (auto& connV : connectedComponent) {
				sortedResult.first.push_back(connV);
			}
			sort(sortedResult.first.begin(), sortedResult.first.end(), vertexInfoPtrComparer());
		}
		sort(outputs.begin(), outputs.end(), minSpinnOutputComparer());

		int counter = 1;
		for (auto& output : outputs) {
			double minSum = minSpinningTreeSumMap[output.second];
			printf("Connected Component { %d}: %.3f\n", counter, minSum);
 			counter++;
		}
	}



	int graph::getVerticesCount() const {
		return vertices.size();
	}



	int graph::getTotalNodeCount() const {
		int totalNodeCount = 0;
		for (const auto& v : vertices)
			totalNodeCount += v.second->adjList.size();
		return totalNodeCount;
	}



	void graph::getStudents(vector<string>& output) const {
		output.clear();
		for (auto& pv : vertices)
			output.push_back(pv.first);
		sort(output.begin(), output.end());
	}



	void graph::clearAll() {
		for (auto v : vertices)
			if (v.second != NULL)
				delete v.second;
		for (auto& connComp : connectedComponents)
			delete connComp;
		vertices.clear();
		connectedComponentMap.clear();
		connectedComponents.clear();
		longestpathWeights.clear();
		shortpathWeights.clear();
		minSpinningTreeSumMap.clear();
	}



	graph::~graph() {
		for (auto& v : vertices)
			if (v.second != NULL) {
				delete v.second;
			}
		for (auto& connComp : connectedComponents)
			delete connComp;
	}



	void graph::calculateConnectedComponents(vertexInfo* ptrCalculateVertex) {
		// stackVertex用來儲存目前運算頂點，
		stack<vertexInfo*> stackVertex;
		// stackIter是用來保存vertexInfo內adjList的疊代器，
		// 用來知道目前讀取到adjList的哪個元素，以及可以知道讀完這個後下一個元素的疊代器，亦可以判斷目前是否已將所有adjList內的元素讀取完畢。
		stack<vertexInfo::adjListIterator> stackIter;
		// visitedVertices用來保存走訪過的節點
		unordered_set<vertexInfo*> visitedVertices;
		// ptrConnectedComponent: 指向連通成分集合的指標
		unordered_set<vertexInfo*>* ptrConnectedComponent = connectedComponentMap[ptrCalculateVertex];
		if (ptrConnectedComponent == NULL) { // 表示目前輸入的節點還沒計算過連通成分
			ptrConnectedComponent = new unordered_set<vertexInfo*>();
			connectedComponentMap[ptrCalculateVertex] = ptrConnectedComponent;
			connectedComponents.push_back(ptrConnectedComponent);
			ptrConnectedComponent->insert(ptrCalculateVertex);
			if (ptrCalculateVertex != NULL) {
				stackVertex.push(ptrCalculateVertex);
				stackIter.push(ptrCalculateVertex->adjList.begin());
			}
			while (!stackVertex.empty()) {
				vertexInfo* topVertex = stackVertex.top();
				vertexInfo::adjListIterator& topIter = stackIter.top(); // 取得目前topVertex讀到的相鄰串列節點的疊代器，(*topIter)會是目前topVertex讀到的相鄰串列節點
				if (topIter == topVertex->adjList.end()) { // 判定topVertex內的相鄰串列節點是否已讀完。
					stackVertex.pop();
					stackIter.pop();
				}
				else {
					vertexInfo* nextVertex = (*topIter).pVertex;
					if (visitedVertices.find(nextVertex) == visitedVertices.end()) {
						stackVertex.push(nextVertex);
						stackIter.push(nextVertex->adjList.begin());
						visitedVertices.insert(nextVertex);
						ptrConnectedComponent->insert(nextVertex);
						connectedComponentMap[nextVertex] = ptrConnectedComponent;
					}
					topIter++;
				}
			}
		}
	}



	void graph::findShortestPath(vertexInfo* ptrStartVertex) {
		unordered_map<vertexInfo*, float>& weight = shortpathWeights[ptrStartVertex];
		if (!weight.empty()) {
			return;
		}
		if (connectedComponentMap[ptrStartVertex] == NULL)
			calculateConnectedComponents(ptrStartVertex);
		unordered_set<vertexInfo*>& connectedComponent = *connectedComponentMap[ptrStartVertex];
		unordered_set<vertexInfo*> cVertexSet; // vertexSet的差集
		for (auto& connVtx : connectedComponent) {
			weight[connVtx] = std::numeric_limits<float>::max();
			cVertexSet.insert(connVtx);
		}
		weight[ptrStartVertex] = 0;

		vertexInfo* curVtx = ptrStartVertex;

		while (!cVertexSet.empty()) {
			cVertexSet.erase(curVtx);
			float curWeight = weight[curVtx];
			float minWeight = std::numeric_limits<float>::max();
			vertexInfo* nextVtx = NULL;

			for (auto& adjNode : curVtx->adjList) {
				float tmp = curWeight + adjNode.weight;
				if (tmp < weight[adjNode.pVertex]) {
					weight[adjNode.pVertex] = tmp;
				}
			}

			for (auto& v : cVertexSet) {
				if (weight[v] < minWeight) {
					minWeight = weight[v];
					nextVtx = v;
				}
			}
			curVtx = nextVtx;
		}
	}




	void graph::findLongestPath(vertexInfo* ptrStartVertex) {
		unordered_map<vertexInfo*, float>& weight = shortpathWeights[ptrStartVertex];
		if (!weight.empty()) {
			return;
		}
		if (connectedComponentMap[ptrStartVertex] == NULL)
			calculateConnectedComponents(ptrStartVertex);
		unordered_set<vertexInfo*>& connectedComponent = *connectedComponentMap[ptrStartVertex];
		unordered_set<vertexInfo*> cVertexSet; // vertexSet的差集
		for (auto& connVtx : connectedComponent) {
			weight[connVtx] = std::numeric_limits<float>::max();
			cVertexSet.insert(connVtx);
		}
		weight[ptrStartVertex] = 0;

		vertexInfo* curVtx = ptrStartVertex;

		while (!cVertexSet.empty()) {
			cVertexSet.erase(curVtx);
			float curWeight = weight[curVtx];
			float maxWeight = std::numeric_limits<float>::min();
			vertexInfo* nextVtx = NULL;

			for (auto& adjNode : curVtx->adjList) {
				float tmp = curWeight + adjNode.weight;
				if (tmp > weight[adjNode.pVertex]) {
					weight[adjNode.pVertex] = tmp;
				}
			}

			for (auto& v : cVertexSet) {
				if (weight[v] > maxWeight) {
					maxWeight = weight[v];
					nextVtx = v;
				}
			}
			curVtx = nextVtx;
		}
	}



	void graph::finsMinSpinningTree(vertexInfo* ptrStartVertex)	{
		vertexInfoPtrComparer vertexInfoCmp;
		if (connectedComponentMap[ptrStartVertex] == NULL)
			calculateConnectedComponents(ptrStartVertex);
		if (minSpinningTreeSumMap.find(connectedComponentMap[ptrStartVertex]) != minSpinningTreeSumMap.end()) {
			return;
		}
		unordered_set<vertexInfo*>& connectedComponent = *connectedComponentMap[ptrStartVertex];
		unordered_map<vertexInfo*, int> vertexGroupMap;
		unordered_map<int, unordered_set<vertexInfo*>> groupMap;
		int groupID = 1;
		priority_queue<pair<double, edge>> priQue;
		vector< pair<double, edge> > minSpinnEdges;
		for (auto& vertex : connectedComponent) {
			for (auto& adjVertexNode : vertex->adjList) {
				bool isInCC = connectedComponent.find(adjVertexNode.pVertex) != connectedComponent.end();
				if (isInCC && vertexInfoCmp(vertex, adjVertexNode.pVertex)) {
					priQue.push(pair<double, edge>(-adjVertexNode.weight, edge(vertex, adjVertexNode.pVertex)));
				}
			}
		}
		double minSpinning = 0;
		while (minSpinnEdges.size() < (connectedComponent.size() - 1) && !priQue.empty()) {
			pair<double, edge> edgeData = priQue.top();
			priQue.pop();
			int vertesAGroup = vertexGroupMap[edgeData.second.first];
			int vertesBGroup = vertexGroupMap[edgeData.second.second];
			if (vertesAGroup != 0 && vertesAGroup == vertesBGroup)
				continue;
			if (vertesAGroup == 0 && vertesBGroup == 0) {
				int newGroup = groupID++;
				vertexGroupMap[edgeData.second.first] = vertexGroupMap[edgeData.second.second] = newGroup;
				groupMap[newGroup].insert(edgeData.second.first);
				groupMap[newGroup].insert(edgeData.second.second);
			}
			else if (vertesAGroup == 0 && vertesBGroup != 0) {
				vertexGroupMap[edgeData.second.first] = vertexGroupMap[edgeData.second.second];
				groupMap[vertesBGroup].insert(edgeData.second.first);
			}
			else if (vertesAGroup != 0 && vertesBGroup == 0) {
				vertexGroupMap[edgeData.second.second] = vertexGroupMap[edgeData.second.first];
				groupMap[vertesAGroup].insert(edgeData.second.second);
			}
			else {
				unordered_set<vertexInfo*>& groupA = groupMap[vertesAGroup];
				unordered_set<vertexInfo*>& groupB = groupMap[vertesBGroup];
				if (groupA.size() >= groupB.size()) {
					unordered_set<vertexInfo*>& insGroup = groupA;
					unordered_set<vertexInfo*>& delGroup = groupB;
					for (auto& groupEle : delGroup) {
						vertexGroupMap[groupEle] = vertesAGroup;
						insGroup.insert(groupEle);
					}
					delGroup.clear();
				}
				else if (groupA.size() < groupB.size()) {
					unordered_set<vertexInfo*>& insGroup = groupB;
					unordered_set<vertexInfo*>& delGroup = groupA;
					for (auto& groupEle : delGroup) {
						vertexGroupMap[groupEle] = vertesBGroup;
						insGroup.insert(groupEle);
					}
					delGroup.clear();
				}
			}
			minSpinning += (-edgeData.first);
			minSpinnEdges.push_back(edgeData);
		}
		minSpinningTreeSumMap[connectedComponentMap[ptrStartVertex]] = minSpinning;
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
	bool adjListNode::operator==(const adjListNode& node) const {
		return pVertex == node.pVertex && pVertex != NULL;
	}
	bool adjListNode::comparer::operator()(const adjListNode& nodeA, const adjListNode& nodeB) const {
		if (nodeA.pVertex == NULL || nodeB.pVertex == NULL)
			return false;
		return nodeA.pVertex->stID.compare(nodeB.pVertex->stID) < 0;
	}
	size_t adjListNode::hasher::operator()(const adjListNode& nodeA) const {
		if (nodeA.pVertex == NULL)
			return 0xFFFFFFFF;
		return hash<string>()(nodeA.pVertex->stID);
	}
	bool studentGraph::vertexInfoPtrComparer::operator()(vertexInfo* const nodeA, vertexInfo* const nodeB) const
	{
		return nodeA->stID.compare(nodeB->stID) < 0;
	}


	bool connectComponentOutputComparer::operator()(const vector<vertexInfo*>& nodeA, const vector<vertexInfo*>& nodeB) const {
		if (nodeA.size() > nodeB.size())
			return true;
		else if (nodeA.size() < nodeB.size())
			return false;
		else
			return nodeA[0]->stID < nodeB[0]->stID;
	}

	bool shortestPathOutputComparer::operator()(const pair<float, vertexInfo*>& item1, pair<float, vertexInfo*>& item2) {
		return item1.first < item2.first;
	}

	bool minSpinnOutputComparer::operator()(const pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>& nodeA, const pair<vector<vertexInfo*>, unordered_set<vertexInfo*>*>& nodeB) const {
		return connectComponentOutputComparer()(nodeA.first, nodeB.first);
	}

}



//Classes that provides the implementation of "special" function.
namespace specialUtility {
	class nyanCatDrawer {
	public:
		nyanCatDrawer() {
			reset();
		}
		void updateFrame() {
			printf("\033[A\033[s");
			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 72; j++) {
					drawPixel(buffer[posMapping(j, i)]);
				}
				fputc('\n', stdout);
			}
			printf("\033[u");
		}
		void drawRect(int x, int y, int w, int h, char c) {
			for (int i = 0; i < h; i++)
				for (int j = 0; j < w; j++) {
					int pos = posMapping(j + x, i + y);
					if (pos >= 0) buffer[pos] = c;
				}
		}
		void drawSinglePixel(int x, int y, int c) {
			drawRect(x, y, 1, 1, c);
		}
		void drawStar(int x, int y, int ani) {
			if (ani == 0) {
				drawSinglePixel(x, y, 1);
			}
			else if (ani == 1) {
				drawSinglePixel(x, y - 1, 1);
				drawSinglePixel(x - 1, y, 1);
				drawSinglePixel(x + 1, y, 1);
				drawSinglePixel(x, y + 1, 1);
			}
			else if (ani == 2) {
				drawRect(x, y - 2, 1, 2, 1);
				drawRect(x - 2, y, 2, 1, 1);
				drawRect(x + 1, y, 2, 1, 1);
				drawRect(x, y + 1, 1, 2, 1);
			}
			else if (ani == 3) {
				drawRect(x, y - 3, 1, 2, 1);

				drawRect(x - 3, y, 2, 1, 1);
				drawRect(x, y, 1, 1, 1);
				drawRect(x + 2, y, 2, 1, 1);

				drawRect(x, y + 2, 1, 2, 1);
			}
			else if (ani == 4) {
				drawSinglePixel(x, y - 3, 1);

				drawSinglePixel(x - 2, y - 2, 1);
				drawSinglePixel(x + 2, y - 2, 1);

				drawSinglePixel(x - 3, y, 1);
				drawSinglePixel(x + 3, y, 1);

				drawSinglePixel(x - 2, y + 2, 1);
				drawSinglePixel(x + 2, y + 2, 1);

				drawSinglePixel(x, y + 3, 1);
			}
			else if (ani == 5) {
				drawSinglePixel(x, y - 2, 1);
				drawSinglePixel(x - 3, y, 1);
				drawSinglePixel(x + 3, y, 1);
				drawSinglePixel(x, y + 2, 1);
			}
		}
		void drawRainbow(int x, int y, int ani) {
			const char colours[] = { 4, 9, 10, 11, 12, 13 };
			for (int i = 0; i < 4; i++) {
				int dx = x + -6 + i * 9;
				for (int j = 0; j < 6; j++) {
					int off = (i & 1) ^ ani;
					int dy = y + off + j * 3;
					drawRect(dx, dy, 9, 3, colours[j]);
				}
			}

		}
		void drawCatHead(int x, int y) {
			for (int i = 0; i < 3; i++) {
				drawRect(x + 2, y + 1 + i, 2 + i, 1, 2);
				drawRect(x + 12 - i, y + 1 + i, 2 + i, 1, 2);
				drawSinglePixel(x + 4 + i, y + 1 + i, 3);
				drawSinglePixel(x + 11 - i, y + 1 + i, 3);
			}
			drawRect(x + 1, y + 4, 14, 7, 2);
			drawRect(x + 2, y + 0, 2, 1, 3);
			drawRect(x + 12, y + 0, 2, 1, 3);
			drawRect(x + 1, y + 1, 1, 4, 3);
			drawRect(x + 14, y + 1, 1, 4, 3);
			drawRect(x + 7, y + 3, 2, 1, 3);
			drawSinglePixel(x + 1, y + 10, 3);
			drawSinglePixel(x + 14, y + 10, 3);
			drawSinglePixel(x + 2, y + 11, 3);
			drawSinglePixel(x + 13, y + 11, 3);
			drawRect(x, y + 5, 1, 5, 3);
			drawRect(x + 15, y + 5, 1, 5, 3);
			drawRect(x + 3, y + 11, 10, 1, 2);
			// Draw Eyes
			drawRect(x + 4, y + 6, 2, 2, 3);
			drawRect(x + 4, y + 6, 1, 1, 1);
			drawRect(x + 11, y + 6, 2, 2, 3);
			drawRect(x + 11, y + 6, 1, 1, 1);
			// Draw Noise
			drawRect(x + 9, y + 7, 1, 1, 3);
			// Draw Mouth
			drawRect(x + 5, y + 9, 1, 1, 3);
			drawRect(x + 8, y + 9, 1, 1, 3);
			drawRect(x + 11, y + 9, 1, 1, 3);
			drawRect(x + 5, y + 10, 7, 1, 3);
			// Draw Blushes
			drawRect(x + 2, y + 8, 2, 2, 7);
			drawRect(x + 13, y + 8, 2, 2, 7);

			drawRect(x + 3, y + 12, 10, 1, 3);
		}
		void drawCatBody(int x, int y) {
			drawRect(x + 2, y, 17, 1, 3);
			drawRect(x + 1, y + 1, 19, 16, 6);
			drawRect(x + 1, y + 1, 1, 1, 3);
			drawRect(x + 19, y + 1, 1, 1, 3);
			drawRect(x, y + 2, 1, 14, 3);
			drawRect(x + 20, y + 2, 1, 14, 3);

			drawRect(x + 1, y + 16, 1, 1, 3);
			drawRect(x + 19, y + 16, 1, 1, 3);
			drawRect(x + 2, y + 17, 17, 1, 3);

			for (int i = 0; i < 2; i++) {
				drawRect(x + 4 - i, y + 2 + i, 13 + (i << 1), 1, 5);
				drawRect(x + 4 - i, y + 15 - i, 13 + (i << 1), 1, 5);
			}
			drawRect(x + 2, y + 4, 17, 10, 5);

			// Draw Square Points
			drawSinglePixel(x + 9, y + 3, 8);
			drawSinglePixel(x + 12, y + 3, 8);
			drawSinglePixel(x + 4, y + 4, 8);
			drawSinglePixel(x + 16, y + 5, 8);
			drawSinglePixel(x + 7, y + 7, 8);
			drawSinglePixel(x + 5, y + 9, 8);
			drawSinglePixel(x + 9, y + 10, 8);
			drawSinglePixel(x + 3, y + 11, 8);
			drawSinglePixel(x + 7, y + 13, 8);
			drawSinglePixel(x + 4, y + 14, 8);


		}
		void drawCatTail(int x, int y, int ani) {
			if (ani == 0) {
				// Draw Borders
				drawRect(x - 5, y - 6, 4, 1, 3);
				drawRect(x - 5, y - 5, 5, 1, 3);
				drawRect(x - 5, y - 4, 6, 1, 3);
				drawRect(x - 4, y - 3, 5, 1, 3);
				drawRect(x - 3, y - 2, 4, 1, 3);
				drawRect(x - 2, y - 1, 3, 1, 3);
				drawRect(x, y, 1, 1, 3);
				// Draw Fills
				drawRect(x - 4, y - 5, 2, 1, 2);
				drawRect(x - 3, y - 4, 2, 1, 2);
				drawRect(x - 2, y - 3, 2, 1, 2);
				drawRect(x - 1, y - 2, 2, 1, 2);
			}
			else if (ani == 1 || ani == 5) {
				// Draw Borders
				drawRect(x - 4, y - 5, 2, 1, 3);
				drawRect(x - 5, y - 4, 4, 1, 3);
				drawRect(x - 5, y - 3, 6, 1, 3);
				drawRect(x - 4, y - 2, 5, 1, 3);
				drawRect(x - 3, y - 1, 4, 1, 3);
				drawRect(x - 1, y, 2, 1, 3);
				// Draw Fills
				drawRect(x - 4, y - 4, 2, 1, 2);
				drawRect(x - 4, y - 3, 2, 1, 2);
				drawRect(x - 3, y - 2, 4, 1, 2);
				drawRect(x - 1, y - 1, 2, 1, 2);
			}
			else if (ani == 2) {
				// Draw Borders
				drawRect(x, y - 3, 1, 1, 3);
				drawRect(x - 3, y - 2, 4, 1, 3);
				drawRect(x - 5, y - 1, 6, 1, 3);
				drawRect(x - 5, y, 6, 1, 3);
				drawRect(x - 4, y + 1, 4, 1, 3);
				// Draw Fills
				drawRect(x - 3, y - 1, 4, 1, 2);
				drawRect(x - 4, y, 3, 1, 2);
			}
			else if (ani == 3) {
				// Draw Borders
				drawRect(x - 1, y - 3, 2, 1, 3);
				drawRect(x - 3, y - 2, 4, 1, 3);
				drawRect(x - 4, y - 1, 5, 1, 3);
				drawRect(x - 5, y, 6, 1, 3);
				drawRect(x - 5, y + 1, 4, 1, 3);
				drawRect(x - 4, y + 2, 2, 1, 3);
				// Draw Fills
				drawRect(x - 1, y - 2, 2, 1, 2);
				drawRect(x - 3, y - 1, 4, 1, 2);
				drawRect(x - 4, y, 2, 1, 2);
				drawRect(x - 4, y + 1, 2, 1, 2);
			}
			else if (ani == 4) {
				// Draw Borders
				drawRect(x - 5, y - 5, 4, 1, 3);
				drawRect(x - 6, y - 4, 7, 1, 3);
				drawRect(x - 6, y - 3, 7, 1, 3);
				drawRect(x - 4, y - 2, 5, 1, 3);
				drawRect(x - 1, y - 1, 2, 1, 3);
				// Draw Fills
				drawRect(x - 5, y - 4, 3, 1, 2);
				drawRect(x - 4, y - 3, 4, 1, 2);
				drawRect(x, y - 2, 1, 1, 2);
			}
		}
		void drawCatFoot1(int x, int y, int ani) {
			if (ani == 0) {
				// Draw Borders
				drawRect(x - 2, y - 1, 3, 1, 3);
				drawRect(x - 3, y, 5, 1, 3);
				drawRect(x - 3, y + 1, 5, 1, 3);
				drawRect(x - 3, y + 2, 4, 1, 3);
				// Draw Fills
				drawRect(x - 2, y, 3, 1, 2);
				drawRect(x - 2, y + 1, 2, 1, 2);
			}
			else if (ani == 1 || ani == 3) {
				// Draw Borders
				drawRect(x - 1, y - 1, 2, 1, 3);
				drawRect(x - 2, y, 4, 1, 3);
				drawRect(x - 2, y + 1, 4, 1, 3);
				drawRect(x - 2, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x - 1, y, 2, 2, 2);
			}
			else if (ani == 2) {
				// Draw Borders
				drawRect(x - 1, y - 1, 2, 1, 3);
				drawRect(x - 1, y, 3, 1, 3);
				drawRect(x - 1, y + 1, 4, 1, 3);
				drawRect(x - 1, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x, y, 1, 1, 2);
				drawRect(x, y + 1, 2, 1, 2);
			}
			else if (ani == 4) {
				// Draw Borders
				drawRect(x - 2, y - 2, 2, 1, 3);
				drawRect(x - 3, y - 1, 4, 1, 3);
				drawRect(x - 4, y, 5, 1, 3);
				drawRect(x - 4, y + 1, 4, 1, 3);
				drawRect(x - 4, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x - 3, y, 3, 1, 2);
				drawRect(x - 3, y + 1, 2, 1, 2);
			}
			else if (ani == 5) {
				// Draw Borders
				drawRect(x - 2, y - 2, 2, 1, 3);
				drawRect(x - 3, y - 1, 4, 1, 3);
				drawRect(x - 4, y, 5, 1, 3);
				drawRect(x - 4, y + 1, 4, 1, 3);
				drawRect(x - 4, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x - 2, y - 1, 1, 1, 2);
				drawRect(x - 3, y, 3, 1, 2);
				drawRect(x - 3, y + 1, 2, 1, 2);
			}
		}
		void drawCatFoot2(int x, int y, int ani) {
			if (ani == 0) {
				// Draw Borders
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
			else if (ani == 1 || ani == 3) {
				// Draw Borders
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x + 1, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
			else if (ani == 2) {
				// Draw Borders
				drawRect(x + 1, y + 1, 4, 1, 3);
				drawRect(x + 2, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 2, y + 1, 2, 1, 2);
			}
			else if (ani == 4) {
				// Draw Borders
				drawRect(x - 2, y + 1, 4, 1, 3);
				drawRect(x - 1, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x - 1, y + 1, 2, 1, 2);
			}
			else if (ani == 5) {
				// Draw Borders
				drawRect(x - 2, y + 1, 4, 1, 3);
				drawRect(x - 2, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x - 1, y + 1, 2, 1, 2);
			}
		}
		void drawCatFoot3(int x, int y, int ani) {
			if (ani == 0 || ani == 4) {
				// Draw Borders
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x + 1, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
			else if (ani == 1 || ani == 3) {
				// Draw Borders
				drawRect(x + 1, y + 1, 4, 1, 3);
				drawRect(x + 2, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 2, y + 1, 2, 1, 2);
			}
			else if (ani == 2) {
				// Draw Borders
				drawRect(x + 2, y + 1, 4, 1, 3);
				drawRect(x + 3, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 3, y + 1, 2, 1, 2);
			}
			else if (ani == 5) {
				// Draw Borders
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
		}
		void drawCatFoot4(int x, int y, int ani) {
			if (ani == 0) {
				// Draw Borders
				drawRect(x, y, 4, 1, 3);
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x + 1, y + 2, 2, 1, 3);
				// Draw Fills
				drawRect(x + 2, y, 1, 1, 2);
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
			else if (ani == 1 || ani == 3) {
				// Draw Borders
				drawRect(x + 1, y, 4, 1, 3);
				drawRect(x + 1, y + 1, 4, 1, 3);
				drawRect(x + 2, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 3, y, 1, 1, 2);
				drawRect(x + 2, y + 1, 2, 1, 2);
			}
			else if (ani == 2) {
				// Draw Borders
				drawRect(x + 2, y, 4, 1, 3);
				drawRect(x + 2, y + 1, 4, 1, 3);
				drawRect(x + 3, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 4, y, 1, 1, 2);
				drawRect(x + 3, y + 1, 2, 1, 2);
			}
			else if (ani == 4 || ani == 5) {
				// Draw Borders
				drawRect(x, y, 4, 1, 3);
				drawRect(x, y + 1, 4, 1, 3);
				drawRect(x + 1, y + 2, 3, 1, 3);
				// Draw Fills
				drawRect(x + 2, y, 1, 1, 2);
				drawRect(x + 1, y + 1, 2, 1, 2);
			}
		}
		void drawCat(int x, int y, int ani) {
			int headOffx = 0, headOffy = 0;
			int allOffy = 0;
			if (ani == 1) {
				headOffx = 1;
			}
			else if (ani == 2 || ani == 3) {
				headOffx = 1;
				allOffy = 1;
			}
			else if (ani == 4) {
				allOffy = 1;
			}
			else if (ani == 5) {
				headOffy = -1;
				allOffy = 1;
			}
			drawCatFoot1(x + 2, allOffy + y + 17, ani);
			drawCatFoot2(x + 5, allOffy + y + 17, ani);
			drawCatFoot3(x + 14, allOffy + y + 17, ani);
			drawCatFoot4(x + 19, allOffy + y + 17, ani);
			drawCatBody(x + 1, allOffy + y);
			drawCatHead(headOffx + x + 11, headOffy + allOffy + y + 5);
			drawCatTail(x, allOffy + y + 13, ani);
		}
		void drawNyanCat(int ani) {
			const int star1Poses[] = { 43, 37, 29, 20, 10, 4, 0, 78, 70, 62, 54, 47 };
			reset();
			drawRainbow(0, 24, ((ani >> 1) & 1) ^ 1);
			drawStar(star1Poses[ani % 12], 1, (ani + 1) % 6); // 43 37 29 20 10 4 0 78 | 70 62 54 47
			drawStar(star1Poses[(ani + 8) % 12], 9, (ani + 9) % 6);
			drawStar(star1Poses[(ani + 6) % 12], 20, (ani + 7) % 6);
			drawStar(star1Poses[(ani + 4) % 12], 40, (ani + 5) % 6);
			drawStar(star1Poses[(ani + 8) % 12], 51, (11 - ani) % 6);
			drawStar(star1Poses[(ani + 1) % 12], 60, (ani + 2) % 6);
			drawCat(28, 23, ani % 6);
		}
		void reset() {
			for (int i = 0; i < 4608; i++)
				buffer[i] = 0;
		}
	private:
		char buffer[4608];

		int posMapping(int x, int y) {
			if (x < 0 || x >= 72 || y < 0 || y >= 64) return -1;
			else return y * 72 + x;
		}
		void drawPixel(char c) {
			char flTx[] = "  ";
			switch (c) {
			case  0: printf("\033[48;2;16;64;128m%s\033[0m", flTx); break;

			case  1: printf("\033[48;2;255;255;255m%s\033[0m", flTx); break;
			case  2: printf("\033[48;2;170;170;170m%s\033[0m", flTx); break;
			case  3: printf("\033[48;2;0;0;0m%s\033[0m", flTx); break;

			case  4: printf("\033[48;2;255;16;16m%s\033[0m", flTx); break;

			case  5: printf("\033[48;2;255;164;255m%s\033[0m", flTx); break;
			case  6: printf("\033[48;2;255;211;161m%s\033[0m", flTx); break;
			case  7: printf("\033[48;2;255;170;170m%s\033[0m", flTx); break;
			case  8: printf("\033[48;2;255;64;170m%s\033[0m", flTx); break;

			case  9: printf("\033[48;2;255;170;16m%s\033[0m", flTx); break;

			case 10: printf("\033[48;2;255;255;8m%s\033[0m", flTx); break;

			case 11: printf("\033[48;2;64;255;16m%s\033[0m", flTx); break;

			case 12: printf("\033[48;2;16;170;255m%s\033[0m", flTx); break;

			case 13: printf("\033[48;2;120;70;255m%s\033[0m", flTx); break;
			}
		}
	};
}
class programStatus {
public:
	studentGraph::graph currentGraph;
	bool haveOpenFile;
	string fileName;
	string outFileName;
	int fileID;
	float threshold;
	programStatus() : haveOpenFile(false), fileID(-1), threshold(0) {

	}

	void reset() {
		currentGraph.clearAll();
		haveOpenFile = false;
		fileName = "";
		fileID = -1;
		threshold = 0;
	}
};

programStatus _programStatus;

void printUsage() {
	printf("===================================================================================\n");
	printf("Usage:");
	printf("\t0) Create Undirection Graph.\n");
	if (_programStatus.haveOpenFile)
		printf("\t1) Calculate connection components.\n");
	else
		printf("\t\033[31m1) Calculate connection components.\033[0m\n");
	if (_programStatus.haveOpenFile)
		printf("\t2) Calculate shortest path by.\n");
	else
		printf("\t\033[31m2) Calculate shortest path by. \033[0m\n");
	if (_programStatus.haveOpenFile)
		printf("\t3) Calculate minimum spinning tree by Kruskal.\n");
	else
		printf("\t\033[31m3) Calculate minimum spinning tree by Kruskal.\033[0m\n");
	printf("\t8) \"Don't use it.\"\n");
	printf("\t9) Exit.\n");
	printf("\tIf you press Ctrl + Z while task ask you to input something, \n\tThis operation will stop current task.\n");
	printf("-----------------------------------------------------------------------------------\n");
	if (_programStatus.haveOpenFile)
		printf("\tCurrent opened file: %s\n", _programStatus.fileName.c_str());
	else
		printf("\tCurrent opened file: \033[31m(none)\033[0m\n");
	if (_programStatus.haveOpenFile)
		printf("\tThreshold: %.3f\n", _programStatus.threshold);
	else
		printf("\tThreshold: \033[31m(none)\033[0m\n");
	printf("===================================================================================\n");
}

void readFileID(const string& message, const string& filenamePrefix, const string& filenameExt, bool& eofToken) {
	using namespace ioUtility;
	using namespace stdioUtility;

	int fileID;
	string fileName;
	float threshold;
	bool success;
	do {
		printf("Enter threshold: ");
		threshold = stdioUtility::readFloat(success, eofToken);
		if (eofToken) {
			printf("You canceled this operation.\n");
			return;
		}
		if (success && (threshold <= 0 || threshold > 1)) {
			printf("\033***   ERROR: [31mThreshold value should be in (0,1].\033[0m   ***\n");
			success = false;
		}
	} while (!success);

	do {
		printf("%s:", message.c_str());
		fileID = readInt32(success, eofToken);
		if (eofToken) {
			printf("You canceled this operation.\n");
			return;
		}
		else if (success) {
			fileName = filenamePrefix + stringUtility::toString(fileID) + filenameExt;
			if (!fileExists(fileName)) {
				printf("*** ERROR: File: %s not found. ***\n", fileName.c_str());
				success = false;
			}
		}
	} while (!success);

	if (!eofToken) {
		_programStatus.reset();
		_programStatus.fileID = fileID;
		_programStatus.fileName = fileName;
		_programStatus.threshold = threshold;
		_programStatus.outFileName = stringUtility::formatString("pairs%d_%.2f", fileID, threshold);
	}
}

string choseStudent(bool& eofToken) {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		vector<string> stNames;
		_programStatus.currentGraph.getStudents(stNames);
		printf("\033[32mAvailable students: \033[0m\n");
		int index = 0;
		for (auto& stName : stNames) {
			if (((index % 5) == 0) && index > 0)
				printf("\n");
			printf("\t%9s", stName.c_str());
			index++;
		}
		printf("\n");
		bool success;
		string stID;
		do {
			printf("\033[33mIf you want to exit, please press Ctrl+Z or enter 0.\033[0m\n");
			printf("Please enter student ID: ");
			stID = stdioUtility::readString(success, eofToken);
			size_t notBlankStart = stID.find_first_not_of(' ');
			size_t notBlankEnd = stID.find_last_not_of(' ');
			if (notBlankStart != string::npos && notBlankEnd != string::npos)
				stID = stID.substr(notBlankStart, notBlankEnd - notBlankStart + 1);
			if (eofToken)
				return "";
			if (success && !_programStatus.currentGraph.containsVertex(stID)) {
				if (stID == "0") {
					eofToken = true;
					return "";
				}
				printf("\033[31m[ ID: %s doesn't exist in current graph. ]\033[0m\n", stID.c_str());
				success = false;
			}
		} while (!success);
		return stID;
	}
}

void Task0() {
	using namespace ioUtility;
	using namespace studentGraph;

	bool eofToken;

	readFileID("Enter File ID", "pairs", ".bin", eofToken);
	float threshold = _programStatus.threshold;
	if (eofToken) {
		return;
	}

	ifstream ifs;
	ifs.open(_programStatus.fileName, ifstream::binary);
	streamBinaryReader binReader(ifs);

	char senderIDBuf[10], receiverIDBuf[10];
	float weightBuf;
	while (!binReader.isEnd()) {
		binReader.readChars(senderIDBuf, 10);
		binReader.readChars(receiverIDBuf, 10);
		weightBuf = binReader.readFloat();
		if (weightBuf <= threshold)
			_programStatus.currentGraph.addEdge(senderIDBuf, receiverIDBuf, weightBuf);
	}
	ifs.close();
	_programStatus.haveOpenFile = true;
	_programStatus.currentGraph.writeToFile(stringUtility::formatString("%s.adj", _programStatus.outFileName.c_str()));
	printf("Number of vertices: %d\n", _programStatus.currentGraph.getVerticesCount());
	printf("Number of nodes: %d\n", _programStatus.currentGraph.getTotalNodeCount());
}

void Task1() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		printf("Number of vertices: %d\n", _programStatus.currentGraph.getVerticesCount());
		timerUtility::timer timer;
		timer.start();
		_programStatus.currentGraph.writeConnectComponentToFile(stringUtility::formatString("%s.cc", _programStatus.outFileName.c_str()));
		timer.stop();
		printf("Duration: %.3lfms\n", timer.getDuration());
	}
}

void Task2() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		bool eofToken;
		do {
			string stID = choseStudent(eofToken);
			if (eofToken) {
				return;
			}
			timerUtility::timer timer;
			timer.start();
			_programStatus.currentGraph.writeShortpathToFile(stID, stringUtility::formatString("%s.ds", _programStatus.outFileName.c_str()));
			timer.stop();
			printf("Duration: %.3lfms\n", timer.getDuration());
		} while (!eofToken);
	}
}

void Task3() {
	if (!_programStatus.haveOpenFile) {
		printf("***   \033[31mERROR: Please create graph first through executing Task1.\033[0m   ***\n");
	}
	else {
		timerUtility::timer timer;
		timer.start();
		_programStatus.currentGraph.outMinSpinningTree();
		timer.stop();
		printf("Duration: %.3lfms\n", timer.getDuration());
	}
}


void TaskHide() {
	using namespace std::this_thread;
	using namespace std::chrono;
	using namespace specialUtility;
	using namespace timerUtility;
	printf("\033[1;1H\033[1K");
	printf("\033[2J\033[3J");
	printf("To ensure \"Special\" animation can be played correctly, please scale down the font size.\n");
	for (int i = 0; i < 64; i++)
		printf("|-------------------------------------------------------------Animation Playing Area-----------------------------------------------------------|\n");
	printf("\"Special\" animation only can be played correctly if you see head and tail message without scolling. Press Enter to play.");
	// 確保CodeBlocks的stdout是有緩衝區的，加速printf的運作速度。
	char stdoutBuf[3000];
	setvbuf(stdout, stdoutBuf, _IOFBF, sizeof(stdoutBuf));
	fgetc(stdin);
	printf("\033[1;1H\033[1K");
	printf("\033[2J\033[3J");
	nyanCatDrawer drawer;
	timer frameTimer;
	for (int i = 0; i < 960; i++) {
		frameTimer.start();
		drawer.reset();
		drawer.drawNyanCat(i);
		drawer.updateFrame();
		//fgetc(stdin);
		fflush(stdout);
		frameTimer.stop();
		double waitingTime = (66.666 - frameTimer.getDuration()) * 1000;
		sleep_for(microseconds((int)waitingTime));
	}
	setvbuf(stdout, NULL, _IONBF, 0);
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
		ofs.open(stringUtility::formatString("graph%d.dot", _programStatus.fileID));
		streamBinaryReader binReader(ifs);

		char senderIDBuf[10], receiverIDBuf[10];
		float weightBuf;
		ofs << "graph " << _programStatus.fileID << "{\n";
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
			try {
				printf(">>>>>>>>>>>>  Task 0 Start  <<<<<<<<<<<<\n");
				Task0();
				printf("<<<<<<<<<<<<  Task 0 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("\033[31m!!!!!!!!!!!!  Task 0 Abort  !!!!!!!!!!!!\033[0m\n\n");
			}
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
				printf("\033[31m!!!!!!!!!!!!  Task3 Abort  !!!!!!!!!!!!\033[0m\n\n");
			}
			break;
		case 8:
			TaskHide();
			exitFlag = true;
			printf("\033[2J\033[3J\033[1;1H\033[36mGoodbye!\033[0m\n");
			break;
		case 9:
			exitFlag = true;
			printf("\033[2J\033[3J\033[1;1H\033[32mGoodbye!\033[0m\n");
			break;
		default:
			printf("*** ERROR: Unknown task: %d ***\n", selectTask);
			break;
		}
		fflush(stdout);
	}
	return 0;
}
