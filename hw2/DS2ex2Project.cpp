/*
	Development Environment:

	IDE: 		Visual Studio 2022 Community 17.5, Code-Block 15.12, Code-Block 20.03
	Compiler:	Visual C++ 14, g++, minGW (with -std=c++11)
	Lang-Std:	C++11
	OS:			Windows 11, Ubuntu 22.04
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <vector>
#include <map>
#include <iomanip>
#include <algorithm>

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <ctime>

#define IO_BUFFER_LEN 1024

#define FIELD_SCHOOL_ID 0
#define FIELD_SCHOOL_NAME 1
#define FIELD_DEPARTMENT_ID 2
#define FIELD_DEPARTMENT_NAME 3
#define FIELD_DEPARTMENT_TYPE 4
#define FIELD_LEVEL 5
#define FIELD_STUDENT_COUNT 6
#define FIELD_TEACHER_COUNT 7
#define FIELD_GRADUATION_COUNT 8
#define FIELD_CITY_NAME 9
#define FIELD_EDUCATION_SYSTEM 10

#define COMPARE_A_BEFORE_B -1
#define COMPARE_A_EQUAL_B 0
#define COMPARE_B_BEFORE_A 1

using namespace std;








//Functions to parse string.
namespace stringutil {
	// To present the result of parsing.
	enum class int_parse_result {
		success = 0,			// There is no error while parsing string to integer.
		int_overflow = 1,		// Integer overflow while parsing string to integer.
		no_input = 2,			// There is no any characters in input string.
		wrong_format = 3,		// There contains a non-digit character in input string.
	};
	// Parse integer from string. success will be false if there have any error when prasing.
	// Allow number type: signed integer. Allow number format: 123 1,234 "123" '123' "1,234" '1,234' (123) (1,234)
	int try_parse_to_int32(string input, int_parse_result& exec_result) {
		char last_char = '\0';
		bool have_input = false;
		bool wrong_format = false;
		bool int_overflow = false;
		bool negative_num = false;
		char str_start_token = '\0';
		int output_number = 0;
		for (char read_char : input) {
			if (read_char == '\n') {
				break;
			}
			else if (read_char < 0) {
				wrong_format = true;
				break;
			}
			else if (read_char == '"' || read_char == '\'') {
				if (str_start_token == '\0') {
					if (!have_input) {
						str_start_token = read_char;
					}
					else {
						wrong_format = true;
					}
				}
				else {
					break;
				}
			}
			else if (read_char == '(' && str_start_token == '\0') {
				if (!have_input) {
					str_start_token = read_char;
				}
				else {
					wrong_format = true;
				}
			}
			else if (read_char == ')' && str_start_token == '(') {
				break;
			}
			else if (have_input && read_char == ',') {
				continue;
			}
			else if (isdigit(read_char)) {
				// multi number case
				if (isspace(last_char) && output_number != 0)
					wrong_format = true;
				if (last_char == '-')
					negative_num = true;
				if (output_number > 214748364) {
					int_overflow = true;
				}
				else if (output_number == 214748364 && (negative_num ? (read_char - '0') > 8 : (read_char - '0') > 7)) {
					int_overflow = true;
				}
				output_number *= 10;
				output_number += (read_char - '0');
				have_input = true;
			}
			else if (last_char == '-') {
				if (have_input)
					wrong_format = true;
			}
			else if (!isspace(read_char) && read_char != '-') {
				wrong_format = true;
			}
			last_char = read_char;
		}
		if (!have_input)
			exec_result = int_parse_result::no_input;
		else if (int_overflow)
			exec_result = int_parse_result::int_overflow;
		else if (wrong_format)
			exec_result = int_parse_result::wrong_format;
		else
			exec_result = int_parse_result::success;
		if (negative_num)
			output_number = -output_number;
		return (have_input && !wrong_format && !int_overflow) ? output_number : 0;
	}
	// Allow boolean format: true false (case insensitive)
	bool try_parse_to_boolean(string input, bool& parse_result) {
		string _compare_str;
		bool output = false;
		for (char _compare_char : input) {
			if (_compare_char == ' ' && _compare_str.size() == 0)
				continue;
			if ('A' <= _compare_char && _compare_char <= 'Z') {
				_compare_str += (_compare_char - 'A' + 'a');
			}
			else {
				_compare_str += _compare_char;
			}
		}
		if (_compare_str.size() == 4) {
			if (_compare_str == "true") {
				parse_result = true;
				output = true;
			}
			else {
				parse_result = false;
			}
		}
		else if (_compare_str.size() == 5) {
			if (_compare_str == "false") {
				parse_result = true;
				output = false;
			}
			else {
				parse_result = false;
			}
		}
		return output;
	}
	// Formating string with C-style format.
	string format_string(const char* format, ...) {
		char buffer[300];
		va_list vl;
		va_start(vl, format);
		vsnprintf(buffer, sizeof(buffer), format, vl);
		va_end(vl);
		return string(buffer);
	}
	// Convert int to string. For compatibility.
	string to_string(int value) {
		string output_str;
		if (value == 0x80000000)
			return string("-2147483648");
		if (value < 0) {
			value *= -1;
			output_str += '-';
		}
		int _div_num = 1;
		while ((value / _div_num) >= 10) {
			_div_num *= 10;
		}
		do {
			output_str += (char)((value / _div_num) + '0');
			value = value % _div_num;
			_div_num /= 10;
		} while (_div_num);
		return output_str;
	}
}







//Functions to read what user types from stdin.
namespace stdioutil {
	// Read a signed 32 bits integer from stdin.
	// If there have any errors while parsing, this function will print the reason.
	// eof_token is means that stdin stream was arrived the end.
	int read_int32(bool& success, bool& eof_token) {
		char read_char =  fgetc(stdin);
		char last_char = '\0';
		bool have_input = false;
		bool wrong_format = false;
		bool int_overflow = false;
		bool negative_num = false;
		int output_number = 0;
		while (read_char != '\n' && read_char != EOF) {

			if (isdigit(read_char)) {
				// multi number case
				if (isspace(last_char) && output_number != 0)
					wrong_format = true;
				if (last_char == '-')
					negative_num = true;
				if (output_number > 214748364) {
					int_overflow = true;
				}
				else if (output_number == 214748364 && (negative_num ? (read_char - '0') > 8 : (read_char - '0') > 7)) {
					int_overflow = true;
				}



				output_number *= 10;

				output_number += (read_char - '0');

				have_input = true;



			}

			else if (last_char == '-') {
				if (have_input)
					wrong_format = true;
			}

			else if (!isspace(read_char)) {
				wrong_format = true;
			}
			last_char = read_char;
			read_char = (char)fgetc(stdin);
		}



		if (wrong_format) {
			printf("*** ERROR: Wrong Format. ***\n");
		}
		else if (int_overflow) {
			printf("*** ERROR: Int Overflow ***\n");
		}
		if (negative_num)
			output_number = -output_number;
		eof_token = read_char == EOF;
		success = have_input && !wrong_format && !int_overflow;
		return success ? output_number : 0;
	}
	// Read string from stdin.
	// All strings that over then 200 bytes is not accepted.
	string read_string(bool& success, bool& eof_token) {
		string out_str = "";
		char read_char = (char)fgetc(stdin);
		bool over_size = false;
		bool non_ascii = false;
		while (read_char != '\n' && read_char != EOF) {
			if (over_size || out_str.size() >= 200) {
				over_size = true;
			}
			else if ((read_char >> 7) != 0) {
				non_ascii = true;
			}
			out_str += read_char;
			read_char = (char)fgetc(stdin);
		}
		eof_token = read_char == EOF;
		if (over_size) {
			printf("*** ERROR: the length of input string cannot over than 200 ***\n");
		}
		else if (non_ascii) {
			printf("*** ERROR: there is non-ascii character in input string. ***\n");
		}
		success = !over_size && !non_ascii;
		return success ? out_str : "";
	}
}






//Functions that provides some file operating method which is not provided by C++ std library.
namespace fileioutil {
	// Check whether if the file is exist or not.
	bool file_exists(const string& file_path) {
		ifstream stream;
		stream.open(file_path, ios_base::in);
		bool check_result = stream.good();
		stream.close();
		return check_result;
	}
}




//Classes that provides the implementation of timer.
namespace timerutil {




	// Class that provides the all timer exception
	class timer_exception : exception {
	public:
		// Constructors
		timer_exception() : exception(), exception_msg("") {

		}
		timer_exception(const char* _exception_msg) : exception_msg(_exception_msg) {

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
		timer() : enabled(false), start_cpu_clock(-1), end_cpu_clock(-1) {
		}
		// Start timer.
		void start() {
			enabled = true;
			start_cpu_clock = clock();
		}
		// Stop timer.
		void stop() {
			end_cpu_clock = clock();
			enabled = false;
		}
		// Get duration
		float get_duration() const {
			if (enabled) {
				throw timer_exception("At <" "timer::get_duration" "> : This timer is running. Please ensure this timer was stop.");
			}
			else if (start_cpu_clock == -1) {
				throw timer_exception("At <" "timer::get_duration" "> : This timer is not start yet.");
			}
			float duration = (float)(end_cpu_clock - start_cpu_clock) / CLOCKS_PER_SEC;
			return duration * 1000;
		}
	private:
		clock_t start_cpu_clock;
		clock_t end_cpu_clock;
		bool enabled;
	};
}



//Classes that provides the implementation of CSV-style table reading and writing.
namespace tableutil {


	// It will be thrown if there have any exception when processing csv-style table.
	class table_exception : exception {
	public:
		// Constructors
		table_exception() : exception(), exception_msg("") {

		}
		table_exception(const char* _exception_msg) : exception_msg(_exception_msg) {

		}
		// Overloading virutal functions
		const char* what() const noexcept {
			return exception_msg;
		}
	private:
		const char* exception_msg;
	};



	// To present a object in table.
	class table_object {
	public:
		// Constructors
		table_object() : base_string(string("")), pre_converted_value(0), pre_convert_result(stringutil::int_parse_result::success) {

		}
		table_object(const string& source_string) {
			set_value(source_string);
		}
		table_object(int source_integer) {
			set_value(source_integer);
		}
		table_object(bool source_boolean) {
			set_value(source_boolean);
		}
			// Set methods
			// Set the value of this table object.
		void set_value(int value) {
			base_string = stringutil::to_string(value);
			pre_converted_value = value;
			pre_convert_result = stringutil::int_parse_result::success;
		}
		void set_value(const string& value) {
			base_string = value;
			pre_converted_value = stringutil::try_parse_to_int32(base_string, pre_convert_result);
			if (pre_convert_result != stringutil::int_parse_result::success && pre_convert_result != stringutil::int_parse_result::no_input) {
				bool _bool_parse_result = false;
				pre_converted_value = stringutil::try_parse_to_boolean(base_string, _bool_parse_result);
				if (!_bool_parse_result) {
					pre_convert_result = stringutil::int_parse_result::wrong_format;
				}
			}
		}
		void set_value(const bool& value) {
			base_string = value ? string("TRUE") : string("FALSE");
			pre_convert_result = stringutil::int_parse_result::success;
			pre_converted_value = value ? 1 : 0;
		}
		// Get methods
			// Regard this table object as int type, if this object cannot be converted into int, it will throw the exception.
		int as_int() const {
			if (pre_convert_result != stringutil::int_parse_result::success && pre_convert_result != stringutil::int_parse_result::no_input) {
				throw table_exception("At <" "table_object::as_int" "> : This table_object cannot be integer.");
			}
			else {
				return pre_converted_value;
			}
		}
		// Regard this table object as int type, if this object cannot be converted into int, it will set <result> argument to false.
		int as_int(bool& result) const {
			if (pre_convert_result != stringutil::int_parse_result::success && pre_convert_result != stringutil::int_parse_result::no_input) {
				result = false;
			}
			else {
				return pre_converted_value;
			}
		}
		// Regard this table object as int type, if this object cannot be converted into int, it will return 0;
		int as_int_nochk() const {
			return pre_converted_value;
		}
		// Regard this table object as string.
		string as_string() const {
			return base_string;
		}
		// Regard this table object as boolean type.
		bool as_boolean() const {
			if (pre_convert_result != stringutil::int_parse_result::success && pre_convert_result != stringutil::int_parse_result::no_input) {
				throw table_exception("At <" "table_object::as_boolean" "> : This table_object cannot be boolean.");
			}
			else {
				return pre_converted_value != 0;
			}
		}
		// Regard this table object as boolean type.
		bool as_boolean(bool& result) const {
			if (pre_convert_result != stringutil::int_parse_result::success && pre_convert_result != stringutil::int_parse_result::no_input) {
				result = false;
			}
			else {
				return pre_converted_value != 0;
			}
		}
		// Regard this table object as boolean type.
		bool as_boolean_nochk() const {
			return pre_converted_value != 0;
		}

	private:
		//Members
		string base_string;			// The base string of table object.
		int pre_converted_value;	// When constructing table_object, its constructor will try to convert source_string to signed 32bit integer and put converted integer to here.
		stringutil::int_parse_result pre_convert_result;	// pre-convert result.
	};







	// To present a row in table.
	class table_row {
	public:
		// Classes
		class read_indicator {
		public:
			read_indicator(const table_row& base_row) : base_table_row(base_row), position(0) {

			}
			read_indicator operator ++ (int) const {
				read_indicator __read_indicator = (*this);
				if(!is_end())
					__read_indicator.position++;
				return __read_indicator;
			}
			read_indicator& operator ++ () {
				read_indicator& __read_indicator = (*this);
				if (!is_end())
					__read_indicator.position++;
				return __read_indicator;
			}
			read_indicator operator >> (int& value) const {
				if (is_end()) {
					value = 0;
				}
				else {
					value = base_table_row[position].as_int_nochk();
				}
				read_indicator __read_indicator = (*this);
				return __read_indicator++;
			}
			read_indicator operator >> (string& value) const {
				if (is_end()) {
					value = "";
				}
				else {
					value = base_table_row[position].as_string();
				}
				read_indicator __read_indicator = (*this);
				return __read_indicator++;
			}
			read_indicator operator >> (bool& value) const {
				if (is_end()) {
					value = false;
				}
				else {
					value = base_table_row[position].as_boolean_nochk();
				}
				read_indicator __read_indicator = (*this);
				return __read_indicator++;
			}
		private:
			// Members
			const table_row& base_table_row;
			int position;
			// Methods
			bool is_end() const{
				return position >= base_table_row.get_column_count();
			}
		};
		// Constructors
		table_row() : base_row_container(vector<table_object>()), column_count(0) {
		}
		// Set methods
		// Set a column to a number given at argument.
		void set_column(int column_offset, int new_value) {
			set_column(column_offset, table_object(new_value));
		}
		// Set a column to a string given at argument.
		void set_column(int column_offset, const string& new_value) {
			set_column(column_offset, table_object(new_value));
		}
		void set_column(int column_offset, const table_object& new_value) {
			if (column_offset >= get_column_count()) {
				base_row_container.resize(column_offset, table_object());
			}
			base_row_container[column_offset] = new_value;
			column_count = (int)base_row_container.size();
		}
		// Get methods
		const table_object& get_column(int column_offset) const {
			if (column_offset >= get_column_count()) {
				throw table_exception("At <" "table_row::get_column" "> : column_offset is greater than or equal to column count.");
			}
			else {
				return base_row_container[column_offset];
			}
		}
		const table_object& get_column(int column_offset, int default_value) const {
			if (column_offset >= get_column_count()) {
				return table_object(default_value);
			}
			else {
				return base_row_container[column_offset];
			}
		}
		const table_object& get_column(int column_offset, const string& default_value) const {
			if (column_offset >= get_column_count()) {
				return table_object(default_value);
			}
			else {
				return base_row_container[column_offset];
			}
		}
		const table_object& get_column(int column_offset, const table_object& default_value) const {
			if (column_offset >= get_column_count()) {
				return default_value;
			}
			else {
				return base_row_container[column_offset];
			}
		}
		int get_column_count() const {
			return (int)column_count;
		}
		// Column operating methods
		void insert_column(int column_offset, int new_object) {
			if (column_offset >= get_column_count()) {
				base_row_container.resize((size_t)column_offset, table_object());
			}
			else {
				base_row_container.push_back(table_object());
				for (int i = get_column_count(); i > column_offset; i--) {
					base_row_container[i] = base_row_container[i - 1];
				}
				base_row_container[column_offset] = table_object(new_object);
			}
			column_count = (int)base_row_container.size();
		}
		void insert_column(int column_offset, const string& new_object) {
			if (column_offset >= get_column_count()) {
				base_row_container.resize((size_t)column_offset, table_object());
			}
			else {
				base_row_container.push_back(table_object());
				for (int i = get_column_count(); i > column_offset; i--) {
					base_row_container[i] = base_row_container[i - 1];
				}
				base_row_container[column_offset] = table_object(new_object);
			}
			column_count = (int)base_row_container.size();
		}
		void insert_column(int column_offset, const table_object& new_object) {
			if (column_offset >= get_column_count()) {
				base_row_container.resize((size_t)column_offset, table_object());
			}
			else {
				base_row_container.push_back(table_object());
				for (int i = get_column_count(); i > column_offset; i--) {
					base_row_container[i] = base_row_container[i - 1];
				}
				base_row_container[column_offset] = new_object;
			}
			column_count = (int)base_row_container.size();
		}
		void append_column(int new_object) {
			base_row_container.push_back(table_object(new_object));
			column_count = (int)base_row_container.size();
		}
		void append_column(const string& new_object) {
			base_row_container.push_back(table_object(new_object));
			column_count = (int)base_row_container.size();
		}
		void append_column(const table_object& new_object) {
			base_row_container.push_back(new_object);
			column_count = (int)base_row_container.size();
		}
		void clear() {
			base_row_container.clear();
			column_count = (int)base_row_container.size();
		}
		// Overloading operators
		const table_object& operator [](int column_offset) const {
			if (column_offset >= base_row_container.size())
				return table_object();
			return base_row_container[column_offset];
		}
		table_object& operator [](int column_offset) {
			return base_row_container[column_offset];
		}
		table_row& operator <<(const int& new_object) {
			base_row_container.push_back(table_object(new_object));
			column_count = (int)base_row_container.size();
			return *(this);
		}
		table_row& operator <<(const string& new_object) {
			base_row_container.push_back(table_object(new_object));
			column_count = (int)base_row_container.size();
			return *(this);
		}
		table_row& operator <<(const bool& new_object) {
			base_row_container.push_back(table_object(new_object));
			column_count = (int)base_row_container.size();
			return *(this);
		}
		table_row& operator <<(const table_object& new_object) {
			base_row_container.push_back(new_object);
			column_count = (int)base_row_container.size();
			return *(this);
		}
		table_row::read_indicator operator >>(int& value) const {
			read_indicator indicator = read_indicator(*this);
			return indicator >> value;
		}
		table_row::read_indicator operator >>(string& value) const {
			read_indicator indicator = read_indicator(*this);
			return indicator >> value;
		}
		table_row::read_indicator operator >>(bool& value) const {
			read_indicator indicator = read_indicator(*this);
			return indicator >> value;
		}
	private:
		// Members
		vector<table_object> base_row_container;

		int column_count;
	};






	// To present a table instance.
	class table {
	public:
		// Constructors
		table() : base_container(vector<table_row>()) {
			base_container.push_back(table_row());
		}
		// File processing methods
		void read_from_file(const string& filename, char separator, bool with_header) {
			if (!fileioutil::file_exists(filename)) {
				throw table_exception("At <" "table::read_from_file" "> : file cannot be found.");
			}
			else {
				ifstream file_stream;
				file_stream.open(filename);
				read_from_stream(file_stream, separator, with_header);
				file_stream.close();
			}
		}
		void read_from_stream(istream& base_stream, char separator, bool with_header) {
			stack<parse_state> _state_stack = stack<parse_state>();
			// Buffers
			table_row _row_buffer = table_row();
			string _str_buffer = string();
			string _space_buffer = string();
			char _bracket_end_char = '\0';
			char _previous_char = '\0';
			_state_stack.push(parse_state::none);
			while (!base_stream.eof()) {
				char _read_buffer[IO_BUFFER_LEN];
				base_stream.read(_read_buffer, IO_BUFFER_LEN);
				int _read_count = (int)base_stream.gcount();
				if (_read_count < IO_BUFFER_LEN) {
					_read_buffer[_read_count++] = '\0';
				}
				for (int i = 0; i < _read_count; i++) {
					char _read_char = _read_buffer[i];
					if (_previous_char == '\r' && _read_char != '\n') { // For macOS Newline: \r
						if (_space_buffer.size() != 0)
							_str_buffer += _space_buffer;
						if (_row_buffer.get_column_count() > 0 || _str_buffer.size() > 0)
							_row_buffer << _str_buffer;
						if (_row_buffer.get_column_count() > 0)
							base_container.push_back(_row_buffer);
						_row_buffer.clear();
						_str_buffer.clear();
						_space_buffer.clear();
					}
					switch (_state_stack.top()) {
					case parse_state::none:
						if (_read_char == '"' || _read_char == '\'') {
							_state_stack.push(parse_state::bracket_string);
							_bracket_end_char = _read_char;
							_space_buffer.clear();
						}
						else if (_read_char == separator) {
							if (_space_buffer.size() != 0)
								_str_buffer += _space_buffer;
							_row_buffer << _str_buffer;
							_str_buffer.clear();
							_space_buffer.clear();
						}
						else if (_read_char == '\n' || _read_char == '\0') { // For Windows Newline: \r\n and Linux Newline: \n
							if (_row_buffer.get_column_count() > 0 || _str_buffer.size() > 0)
								_row_buffer << _str_buffer;
							if (_row_buffer.get_column_count() > 0)
								base_container.push_back(_row_buffer);
							_str_buffer.clear();
							_row_buffer.clear();
							_space_buffer.clear();
						}
						else if (_read_char == '\r') {

						}
						else if (_read_char == ' ') {
							_space_buffer += _read_char;
						}
						else if (_str_buffer.size() == 0) {
							_state_stack.push(parse_state::standard_string);
							_str_buffer += _space_buffer;
							_str_buffer += _read_char;
							_space_buffer.clear();
						}
						else {
							fprintf(stderr, "EXCEPTION: At <%s> : unexcepted character: %c\n", __FUNCTION__, _read_char);
							throw table_exception("At <" "table::read_from_stream" "> : unexcepted character.");
						}
						break;
					case parse_state::standard_string:
						if (_read_char == separator) {
							_row_buffer << _str_buffer;
							_str_buffer.clear();
							_state_stack.pop();
						}
						else if (_read_char == '\n' || _read_char == '\0') {
							if (_row_buffer.get_column_count() > 0 || _str_buffer.size() > 0)
								_row_buffer << _str_buffer;
							if (_row_buffer.get_column_count() > 0)
								base_container.push_back(_row_buffer);
							_str_buffer.clear();
							_row_buffer.clear();
							_state_stack.pop();
						}
						else if (_read_char == '\r') {

						}
						else {
							_str_buffer += _read_char;
						}
						break;
					case parse_state::bracket_string:
						if (_read_char == _bracket_end_char) {
							_state_stack.pop();
							_bracket_end_char = '\0';
						}
						else {
							_str_buffer += _read_char;
						}
						break;
					}
					_previous_char = _read_char;
				}

			}
			if (with_header && base_container.size() > 1)
				base_container.erase(base_container.begin());
			if (_state_stack.top() != parse_state::none) {
				fprintf(stderr, "EXCEPTION: At <%s> : incorrect state.\n", __FUNCTION__);
				throw table_exception("At <" "table::read_from_stream" "> : incorrect state.");
			}
		}
		void save_to_file(const string& filename, char separator, bool with_header, bool autofit = false) {
			ofstream file_stream;
			file_stream.open(filename);
			save_to_stream(file_stream, separator, with_header, autofit);
			file_stream.close();
		}
		void save_to_stream(ostream& base_stream, char separator, bool with_header, bool autofit = false) {
			int _initial_index = with_header ? 0 : 1;
			if (autofit) {
				vector<int> _max_width;
				for (int i = _initial_index; i < base_container.size(); i++) {
					const table_row& _output_row = base_container[i];
					for (int j = 0; j < _output_row.get_column_count(); j++) {
						int _str_len = _output_row.get_column(j).as_string().size();
						if (_max_width.size() <= j) {
							_max_width.push_back(_str_len);
						}
						else {
							if (_max_width[j] < _str_len)
								_max_width[j] = _str_len;
						}
					}
				}
				for (int i = _initial_index; i < base_container.size(); i++) {
					const table_row& _output_row = base_container[i];
					if (i > _initial_index)
						base_stream << '\n';
					for (int j = 0; j < _output_row.get_column_count(); j++) {
						if (j)
							base_stream << separator;
						string output_str = _output_row.get_column(j).as_string();
						output_str.resize(_max_width[j], ' ');
						base_stream << output_str;
					}
				}
			}
			else {
				for (int i = _initial_index; i < base_container.size(); i++) {
					const table_row& _output_row = base_container[i];
					if (i > _initial_index)
						base_stream << '\n';
					for (int j = 0; j < _output_row.get_column_count(); j++) {
						if (j)
							base_stream << separator;
						base_stream << _output_row.get_column(j).as_string();
					}
				}
			}
		}
		// Set methods
		void set_row(int row_offset, const table_row& new_value) {
			row_offset++;
			if (row_offset >= base_container.size()) {
				throw table_exception("At <" "table:::set_row" "> : Given row_offset is invalid.");
			}
			else {
				base_container[row_offset] = new_value;
			}
		}
		void set_header(const table_row& new_header) {
			base_container[0] = new_header;
		}
		// Get methods
		table_row get_row(int row_offset) const {
			row_offset++;
			if (row_offset >= base_container.size()) {
				throw table_exception("At <" "table::get_row" "> : Given row_offset is invalid.");
			}
			else {
				return base_container[row_offset];
			}
		}
		table_row get_header() const {
			return base_container[0];
		}
		int get_row_count() const {
			return (int)base_container.size() - 1;
		}
		// Row operating
		void insert_row(int row_offset, const table_row& new_row) {
			row_offset++;
			if (row_offset >= base_container.size()) {
				base_container.resize(row_offset + 1, table_row());
				base_container[row_offset] = new_row;
			}
			else {
				base_container.push_back(table_row());
				for (int i = base_container.size() - 1; i > row_offset; i--) {
					base_container[i] = base_container[i - 1];
				}
				base_container[row_offset] = new_row;
			}
		}
		// Overloading Operators
		const table_row& operator [](int row_offset) const {
			row_offset++;
			if (row_offset >= base_container.size()) {
				throw table_exception("At <" "table::operator[]" "> : Given row_offset is invalid.");
			}
			else {
				return base_container[row_offset];
			}
		}
		table_row& operator [](int row_offset) {
			row_offset++;
			if (row_offset >= base_container.size()) {
				throw table_exception("At <" "table::operator[]" "> : Given row_offset is invalid.");
			}
			else {
				return base_container[row_offset];
			}
		}
		table& operator << (const table_row& new_row) {
			base_container.push_back(new_row);
			return *(this);
		}
	private:
		// Members
		vector<table_row> base_container;
		// Enum Classes
		enum class parse_state {
			none, standard_string, bracket_string
		};
	};
}





//Classes that provides the implementation of R.O.C Ministry of Education graduation statistics
namespace edu_stat {
	using namespace tableutil;
	class graduation_record {
	public:
		// Member
		int record_id;
		string school_id;
		string school_name;
		string department_id;
		string department_name;
		string department_type;
		string level;
		int student_count;
		int teacher_count;
		int graduation_count;
		string city_name;
		string education_system;
		// Constructor
		graduation_record() {
			record_id = -1;
			student_count = -1;
			teacher_count = -1;
			graduation_count = -1;
		}
		graduation_record(int id, const table_row& base_row): graduation_record(){
			record_id = id;
			base_row >> school_id >> school_name
				>> department_id >> department_name >> department_type
				>> level >> student_count >> teacher_count >> graduation_count >> city_name >> education_system;
		}
	};



	class school_graduation_info {
	public:
		string school_name;
		vector<graduation_record> records;
		school_graduation_info() {
			school_name = "";
		}
		school_graduation_info(string _school_name) {
			school_name = _school_name;
		}
		void push_record(const graduation_record& record) {
			records.push_back(record);
		}
	};
}





//Classes that provides the implementation of Tree
namespace tree_util {



	// Function pointer type, Used to compare the priority of keys.
	// If the priority of val1 is lower to val2, return -1
	// If the priority of val1 is equal to val2, return 0
	// If the priority of val1 is higher to val2, return 1
	template <class TKey, class TData>
	using comparer = int (*)(const TKey& val1, const TKey& val2);




	template <class TKey, class TData>
	class key_pair {
	public:
		// Members
		TKey key;
		TData data;
		// Constructor
		key_pair(const TKey& _key, const TData& _data) {
			key = _key;
			data = _data;
		}
	};




	template <class TKey, class TData>
	class twothree_tree {
	public:
		class twothree_treenode {
		public:
			// Members
			twothree_treenode* parent;
			vector<twothree_treenode*> childs;
			vector<key_pair<TKey,TData>> data;
			// Constructors
			twothree_treenode() {
				parent = NULL;
			}
			// Methods
			// Insert a data into treenode.
			void insert_data(int index, const key_pair<TKey, TData>& new_data) {
				int data_count = get_data_count();
				if (index > data_count)
					throw runtime_error("");
				if (data_count == 0) {
					data.push_back(new_data);
				}
				else {
					data.push_back(data[data_count - 1]);
					for (int i = data_count - 1; i > index; i--) {
						data[i] = data[i - 1];
					}
					data[index] = new_data;
				}
			}
			// Insert a data into treenode.
			void insert_data(int index, const TKey& key, const TData& new_data) {
				insert_data(index, key_pair<TKey, TData>(key, new_data));
			}
			// Insert a child into treenode.
			void insert_child(int index, twothree_treenode* node) {
				int child_count = get_child_count();
				if (index > child_count)
					throw runtime_error("");
				node->parent = this;
				if (child_count == 0) {
					childs.push_back(node);
				}
				else {
					childs.push_back(childs[child_count - 1]);
					for (int i = child_count - 1; i > index; i--) {
						childs[i] = childs[i - 1];
					}
					childs[index] = node;
				}
			}

			// Remove a data from treenode.
			void remove_data(int index) {
				int data_count = get_data_count();
				if (index >= data_count)
					throw runtime_error("");
				for (int i = index + 1; i < data_count; i++) {
					data[i - 1] = data[i];
				}
				data.pop_back();
			}
			// Remove a child from treenode.
			void remove_child(int index) {
				int child_count = get_child_count();
				if (index >= child_count)
					throw runtime_error("");
				twothree_treenode* deleted_node = childs[index];
				for (int i = index + 1; i < child_count; i++) {
					childs[i - 1] = childs[i];
				}
				childs.pop_back();
				delete deleted_node;
			}

			int get_data_count() const{
				return data.size();
			}
			int get_child_count() const {
				return childs.size();
			}

		private:
			
		};
		twothree_tree(comparer<TKey,TData> _comparer) {
			default_value = TData();
			base_comparer = _comparer;
			root = NULL;
		}
		twothree_tree(const TData& _default_value, comparer<TKey, TData> _comparer): default_value(_default_value){
			base_comparer = _comparer;
			root = NULL;
		}
		// Methods
		void push(const TKey& key, const TData& data) {


			//無root
			if (root == NULL) {
				root = new twothree_treenode();
				root->insert_data(0, key, data);
			}



			else {
				int i;
				twothree_treenode* find_node = root;
				while (find_node != NULL) {
					int data_count = find_node->get_data_count();
					for (i = 0; i < data_count; i++) {
						int cmp_result = base_comparer(find_node->data[i].key, key);
																					// cmp_result =  (-1)    find_node->data[i].key(old) < key(new)
						if (cmp_result > 0) {										// cmp_result =  (0)    find_node->data[i].key(old) = key(new)
							break;													// cmp_result =  (1)    find_node->data[i].key(old) > key(new)
						}
						else if (cmp_result == 0) {
							throw runtime_error(""); // delicated key.
						}
					}

					// 看他在哪個子節點
					if (find_node->get_child_count() > i)
						find_node = find_node->childs[i];

					//沒子節點
					else
						break;
				}



				find_node->insert_data(i, key, data); // 把new data 的鍵值放入地i比若有資料就把old data 全部後移




																	//因上行當node可能會有3筆					
				while (find_node != NULL && find_node->get_data_count() >= 3) {

												//他有多少個子節點
					int child_count = find_node->get_child_count();
					twothree_treenode* tmp_parent = find_node->parent;

					// 消失舊node 創2新node
					twothree_treenode* new_left_node = new twothree_treenode();
					twothree_treenode* new_right_node = new twothree_treenode();
					new_left_node->insert_data(0, find_node->data[0]);  //小
					new_right_node->insert_data(0, find_node->data[2]);  //大



					// 三筆資料(以下都執行)  進入此判斷代表作到第二次分裂
					if (child_count >= 1) {
						new_left_node->insert_child(0, find_node->childs[0]);  //     new_left_node_ptr[0] =  find_node->childs[0]_ptr  
					}
					if (child_count >= 2) {
						new_left_node->insert_child(1, find_node->childs[1]);  //     new_left_node_ptr[1] =  find_node->childs[1]_ptr
					}


					if (child_count >= 3) {
						new_right_node->insert_child(0, find_node->childs[2]);  //     new_right_node_ptr[0] =  find_node->childs[2]_ptr
					}
					if (child_count >= 4) {
						new_right_node->insert_child(1, find_node->childs[3]);  //     new_right_node_ptr[1] =  find_node->childs[3]_ptr
					}

					//有父節點
					if (tmp_parent != NULL) {   //判斷此節點在他父節第幾個位址
						for (i = 0; i < tmp_parent->get_child_count(); i++) {
							if (tmp_parent->childs[i] == find_node)
								break;
						}
						tmp_parent->insert_data(i, find_node->data[1]);
						tmp_parent->remove_child(i);
						tmp_parent->insert_child(i, new_left_node);
						tmp_parent->insert_child(i + 1, new_right_node);
						find_node = tmp_parent;
					}
					else {
						root = new twothree_treenode();
						root->insert_data(0, find_node->data[1]);
						root->insert_child(0, new_left_node);
						root->insert_child(1, new_right_node);
						find_node = root;
					}
				}
			}
		}
		twothree_treenode* search_node(const TKey& find_key, int& data_index) {
			twothree_treenode* find_node = root;
			while (find_node != NULL) {
				int i;
				int data_count = find_node->get_data_count();
				for (i = 0; i < data_count; i++) {
					const TKey& cmp_obj = find_node->data[i].key;  //當節點第一筆資料的鍵值
					int cmp_result = base_comparer(cmp_obj, find_key);  //cmp_obj(old) < find_key(new)  == -1
					if (cmp_result > 0) {    //  與現資料比大小			  //cmp_obj(old) = find_key(new)  == 0
						break;											//cmp_obj(old) > find_key(new)  == 1
					}
					else if (cmp_result == 0) {    //相等
						data_index = i;
						return find_node;
					}
				}					//判斷child有多少ptr
				if (find_node->get_child_count() > i) {  // 判斷是在當node  0.1.2 ptr
					find_node = find_node->childs[i];
				}
				else {
					find_node = NULL;
				}
			}
			data_index = -1;
			return find_node;
		}
		TData& search(const TKey& find_key, bool& exists){
			int data_index = 0;
			twothree_treenode* searched_node = search_node(find_key, data_index);
			TData _default_return = TData(default_value);
			if (searched_node == NULL) {
				exists = false;
				return _default_return;			//在23tree無找到 回傳預設值
			}
			else {
				exists = true;
				return searched_node->data[data_index].data;  // 在23tree有找到 回傳reference
			}
		}
		void remove_key(const TKey& key) {
			
		}
		twothree_treenode* get_root() {
			return root;
		}
		// Const Methods
		int get_tree_height() const {
			int height;
			vector<queue<twothree_treenode*>> double_queue = vector<queue<twothree_treenode*>>(2, queue<twothree_treenode*>());
			if (root != NULL) {
				double_queue[0].push(root);
			}
			for (height = 0; double_queue[height & 1].size() > 0; height++) {
				queue<twothree_treenode*>& read_queue = double_queue[height & 1];
				queue<twothree_treenode*>& write_queue = double_queue[(height & 1) ^ 1];
				while (!read_queue.empty()) {
					twothree_treenode* top_ele = read_queue.front();
					read_queue.pop();
					for (int i = 0; i < top_ele->get_child_count(); i++) {
						write_queue.push(top_ele->childs[i]);
					}
				}
			}
			return height;
		}
		int get_node_count() const {
			int total_count = 0;
			vector<queue<twothree_treenode*>> double_queue = vector<queue<twothree_treenode*>>(2, queue<twothree_treenode*>());
			if (root != NULL) {
				double_queue[0].push(root);
			}
			for (int height = 0; double_queue[height & 1].size() > 0; height++) {
				queue<twothree_treenode*>& read_queue = double_queue[height & 1];
				queue<twothree_treenode*>& write_queue = double_queue[(height & 1) ^ 1];
				while (!read_queue.empty()) {
					twothree_treenode* top_ele = read_queue.front();
					total_count++;
					read_queue.pop();
					for (int i = 0; i < top_ele->get_child_count(); i++) {
						write_queue.push(top_ele->childs[i]);
					}
				}
			}
			return total_count;
		}
		bool is_empty() const {
			return root == NULL;
		}


		//[] 
		// Operator override
		TData& operator[](const TKey& key) {
			bool exists;
			TData& foundData = search(key, exists);

			//exist true有 false無
			if (!exists) {
				push(key, default_value);

				//為了再對新放入的資料位置找到位址
				return search(key, exists);
			}
			return foundData;
		}
		const TData& operator[](const TKey& key) const{
			bool exists;
			const TData& foundData = search(key, exists);
			if (foundData == NULL)
				throw runtime_error("");
			return foundData;
		}
		// Destructor
		~twothree_tree() {
			queue < twothree_treenode* > release_queue;
			if(root != NULL)
				release_queue.push(root);
			while (!release_queue.empty()) {
				twothree_treenode* del_node = release_queue.front();
				release_queue.pop();
				if (del_node != NULL) {
					for (int i = 0; i < del_node->get_child_count(); i++) {
						release_queue.push(del_node->childs[i]);
					}
					delete del_node;
				}
			}
		}
	private:
		// Members
		TData default_value;
		comparer<TKey, TData> base_comparer;
		twothree_treenode* root;
	};
	






	template <class TKey, class TData>
	class binary_treenode {
	public:
		// Members
		binary_treenode* parent;
		key_pair<TKey, TData> nodedata;
		binary_treenode* leftchild;
		binary_treenode* rightchild;
		// Constructor
		binary_treenode(const key_pair<TKey, TData>& _nodedata) : nodedata(_nodedata) {
			parent = NULL;
			leftchild = NULL;
			rightchild = NULL;
		}
		binary_treenode(const TKey& key, const TData& data) : nodedata(key_pair<TKey, TData>(key, data)) {
			parent = NULL;
			leftchild = NULL;
			rightchild = NULL;
		}
		// Methods
		void set_leftchild(binary_treenode* left_node) {
			leftchild = left_node;
			if (left_node != NULL)
				left_node->parent = this;
		}
		void set_rightchild(binary_treenode* right_node) {
			rightchild = right_node;
			if (right_node != NULL)
				right_node->parent = this;
		}
		/*	It only use if this node will link to other node after popping.
			If you want to remove leftchild, considering calling remove_leftchild method. */
		binary_treenode* pop_leftchild() {
			binary_treenode* pop_node = leftchild;
			if (pop_node != NULL)
				pop_node->parent = NULL;
			leftchild = NULL;
			return pop_node;
		}
		/*	It only use if this node will link to other node after popping.
			If you want to remove rightchild, considering calling remove_rightchild method. */
		binary_treenode* pop_rightchild() {
			binary_treenode* pop_node = rightchild;
			if (pop_node != NULL)
				pop_node->parent = NULL;
			rightchild = NULL;
			return rightchild;
		}
		/*	It will remove leftchild and its all childs and grandchilds. */
		void remove_leftchild() {
			queue< binary_treenode* > del_queue;
			del_queue.push(leftchild);
			while (!del_queue.empty()) {
				binary_treenode* del_node = del_queue.front();
				del_queue.pop();
				if (del_node != NULL) {
					if (del_node->leftchild != NULL) {
						del_queue.push(del_node->leftchild);
					}
					if (del_node->rightchild != NULL) {
						del_queue.push(del_node->rightchild);
					}
				}
				delete del_node;
			}
			leftchild = NULL;
		}
		/*	It will remove rightchild and its all childs and grandchilds. */
		void remove_rightchild() {
			queue< binary_treenode* > del_queue;
			del_queue.push(rightchild);
			while (!del_queue.empty()) {
				binary_treenode* del_node = del_queue.front();
				del_queue.pop();
				if (del_node != NULL) {
					if (del_node->leftchild != NULL) {
						del_queue.push(del_node->leftchild);
					}
					if (del_node->rightchild != NULL) {
						del_queue.push(del_node->rightchild);
					}
				}
				delete del_node;
			}
			rightchild = NULL;
		}
		
		int get_subtree_height() {
			vector<queue<binary_treenode*>> search_queues = vector<queue<binary_treenode*>>(2, queue<binary_treenode*>());
			int height = 0;
			search_queues[0].push(this);
			for (height = 0; !search_queues[height & 1].empty(); height++) {
				queue<binary_treenode*>& read_queue = search_queues[height & 1];
				queue<binary_treenode*>& write_queue = search_queues[(height & 1) ^ 1];
				while (!read_queue.empty()) {
					binary_treenode* node = read_queue.front();
					read_queue.pop();
					if (node->leftchild != NULL)
						write_queue.push(node->leftchild);
					if (node->rightchild != NULL)
						write_queue.push(node->rightchild);
				}
			}
			return height;
		}

		bool have_leftchild() const {
			return leftchild != NULL;
		}
		bool have_rightchild() const {
			return rightchild != NULL;
		}
		bool is_root() const {
			return parent == NULL;
		}
		bool is_leftnode() const {
			return parent != NULL && parent->leftchild == this;
		}
		bool is_rightnode() const {
			return parent != NULL && parent->rightchild == this;
		}
	};





	// Implement of Binary tree.
	template <class TKey, class TData>
	class binary_tree {
	public:
		// Comparer Type
		//typedef int (*comparer)(const TKey& val1, const TKey& val2);
		// Members
		// Constructor
		binary_tree(comparer<TKey, TData> _comparer) {
			root = NULL;
			default_value = TData();
			base_comparer = _comparer;
		}
		binary_tree(const TData& _default_value, comparer<TKey, TData> _comparer): binary_tree(_comparer) {
			default_value = _default_value;
		}


		// Methods
		TData& search(const TKey& find_key, bool& exists) {
			binary_treenode<TKey, TData>* find_node = search_node(find_key);
			TData _default_value = TData(default_value);
			if (find_node == NULL) {
				exists = false;
				return _default_value;
			}
			else {
				exists = true;
				return find_node->nodedata.data;
			}
		}

		//找節點位址
		binary_treenode<TKey, TData>* search_node(const TKey& find_key) {
			binary_treenode<TKey, TData>* find_node = root;
			while (find_node != NULL) {
				int cmp_result = base_comparer(find_node->nodedata.key, find_key);

				// find_key(new) > find_node(old)
				if (cmp_result < 0) {
					find_node = find_node->rightchild;
				}
				// find_key == find_node
				else if (cmp_result == 0) {
					return find_node;
				}
				// find_key < find_node
				else {
					find_node = find_node->leftchild;
				}
			}
			return NULL;
		}
		// push a data into binary tree.
		// Return: the pointer of new node.
		virtual binary_treenode<TKey, TData>* push(const TKey& key, const TData& data) {
			binary_treenode<TKey, TData>* new_node = new binary_treenode<TKey, TData>(key, data);
			if (root == NULL) {
				root = new_node;
			}
			else {

											// 上last_node -> 下find_node
				binary_treenode<TKey, TData>* find_node = root;
				binary_treenode<TKey, TData>* last_node = root;
				int cmp_result;
				while (find_node != NULL) {
					last_node = find_node;
					cmp_result = base_comparer(find_node->nodedata.key, key);
					// /find_node->nodedata.key(old) < key(new)
					if (cmp_result < 0) {
						find_node = find_node->rightchild;
					}
					// /find_node->nodedata.key(old) == key(new)
					else if (cmp_result == 0) {
						throw runtime_error("key exists");
					}
					// /find_node->nodedata.key(old) > key(new)
					else {
						find_node = find_node->leftchild;
					}
				}
				// /find_node->nodedata.key(old) < key(new)
				if (cmp_result < 0) {
					last_node->set_rightchild(new_node);
				}
				// /find_node->nodedata.key > key
				else if (cmp_result > 0) {
					last_node->set_leftchild(new_node);
				}
			}
			return new_node;
		}


		// Return: the pointer of the node replaced removed node, if the deleted node is leaf, return the pointer of its parent.
		virtual binary_treenode<TKey, TData>* remove_key(const TKey& key) {
			binary_treenode<TKey, TData>* find_node = search_node(key);
			if (!find_node)
				return NULL;
			binary_treenode<TKey, TData>* replace_node = NULL;
			if (find_node->have_rightchild()) {
				replace_node = find_node->rightchild;
				while (replace_node->leftchild != NULL) {
					replace_node = replace_node->leftchild;
				}
			}
			else if (find_node->have_leftchild()) {
				replace_node = find_node->leftchild;
				while (replace_node->rightchild != NULL) {
					replace_node = replace_node->rightchild;
				}
			}
			else {
				replace_node = NULL;
			}
			if (replace_node != NULL) {
				binary_treenode<TKey, TData>* replace_node_child =
					replace_node->have_leftchild() ? replace_node->leftchild 
					: replace_node->have_rightchild() ? replace_node->rightchild 
					: NULL;
				if (replace_node->is_leftnode()) {
					replace_node->parent->set_leftchild(replace_node_child);
					replace_node->parent = NULL;
				}
				else if (replace_node->is_rightnode()) {
					replace_node->parent->set_rightchild(replace_node_child);
					replace_node->parent = NULL;
				}
				replace_node->set_leftchild(find_node->leftchild);
				replace_node->set_rightchild(find_node->rightchild);
			}
			if (find_node->parent != NULL) {
				if (find_node->is_leftnode()) {
					find_node->parent->set_leftchild(replace_node);
				}
				else if (find_node->is_rightnode()) {
					find_node->parent->set_rightchild(replace_node);
				}
			}
			else {
				root = replace_node;
				if(replace_node != NULL)
					replace_node->parent = NULL;
			}
			if (replace_node == NULL)
				replace_node = find_node->parent;
			delete find_node;
			return replace_node;
		}
		int get_tree_height() const{
			if (root != NULL)
				return root->get_subtree_height();
			else
				return 0;
		}
		int get_node_count() const {
			int total_count = 0;
			queue<binary_treenode<TKey, TData>*> search_queue = queue<binary_treenode<TKey, TData>*>();
			if (root != NULL)
				search_queue.push(root);
			while (!search_queue.empty()) {
				binary_treenode<TKey,TData>* node = search_queue.front();
				search_queue.pop();
				if (node != NULL) {
					search_queue.push(node->leftchild);
					search_queue.push(node->rightchild);
					total_count++;
				}
			}
			return total_count;
		}


		binary_treenode<TKey, TData>* get_root(){
			return root;
		}






		// Operator overloading
		TData& operator[](const TKey& key) {
			bool exists;
			TData& output_data = search(key, exists);
			if (!exists) {
				push(key, default_value);
				return search(key, exists);
			}
			else {
				return output_data;
			}
		}


		//
		const TData& operator[](const TKey& key) const{
			bool& exists;
			const TData& output_data = search(key, exists);
			if (!exists) {
				insert_data(key, default_value);
				return search(key, exists);
			}
			else {
				return output_data;
			}
		}
		~binary_tree() {
			if (root != NULL) {
				root->remove_leftchild();
				root->remove_rightchild();
			}
			delete root;
			root = NULL;
		}
	protected:
		// Members
		binary_treenode<TKey,TData>* root;
		comparer<TKey, TData> base_comparer;
		TData default_value;
	};





	// Implement of AVL Tree.
	template <class TKey, class TData>
	class avl_tree : public binary_tree<TKey, TData> {
	public:
		// Constructors
		avl_tree(comparer<TKey, TData> _comparer): binary_tree<TKey, TData>(_comparer){
			
		}
		avl_tree(const TData& _default_data, comparer<TKey, TData> _comparer) : binary_tree<TKey, TData>(_default_data, _comparer) {

		}





		// Methods
		// Override virtual function: push.
		binary_treenode<TKey, TData>* push(const TKey& key, const TData& data) {
			binary_treenode<TKey, TData>* new_node = binary_tree<TKey, TData>::push(key,data);
			balance_tree(new_node);
			return new_node;
		}
		binary_treenode<TKey, TData>* remove_key(const TKey& key) {
			binary_treenode<TKey, TData>* replaced_node = binary_tree<TKey, TData>::remove_key(key);
			if (replaced_node != NULL) {
				if (replaced_node->have_rightchild()) {
					while (replaced_node->leftchild != NULL) {
						replaced_node = replaced_node->leftchild;
					}
				}
				else if (replaced_node->have_leftchild()) {
					while (replaced_node->rightchild != NULL) {
						replaced_node = replaced_node->rightchild;
					}
				}
				balance_tree(replaced_node);
			}
			return replaced_node;
		}
		// Debug
		bool check_avltree() {
			return check_avltreenode(this->root);
		}
		bool check_avltreenode(binary_treenode<TKey, TData>* node) {
			if (node == NULL)
				return true;
			int bf = calculate_balance_factor(node);
			if (bf > 1)
				return false;
			else
				return check_avltreenode(node->leftchild) && check_avltreenode(node->rightchild);
		}
	private:
		binary_treenode<TKey, TData>* rotate_right(binary_treenode<TKey,TData>* node) {
			if (!node->have_rightchild()) {
				return node;
			}
			binary_treenode<TKey, TData>* old_node_right = node->rightchild;
			binary_treenode<TKey, TData>* old_node_right_left = old_node_right->leftchild;
			if (!node->is_root()) {
				if (node->is_leftnode()) {
					node->parent->set_leftchild(old_node_right);
				}
				else {
					node->parent->set_rightchild(old_node_right);
				}
			}
			else {
				old_node_right->parent = NULL;
				this->root = old_node_right;
			}
			old_node_right->set_leftchild(node);
			node->set_rightchild(old_node_right_left);
			return old_node_right;
		}
		binary_treenode<TKey, TData>* rotate_left(binary_treenode<TKey, TData>* node) {
			if (!node->have_leftchild()) {
				return node;
			}
			binary_treenode<TKey, TData>* old_node_left = node->leftchild;
			binary_treenode<TKey, TData>* old_node_left_right = old_node_left->rightchild;
			if (!node->is_root()) { // 此點不是root
				if (node->is_leftnode()) {    // 此點是他父節點的左邊節點
					node->parent->set_leftchild(old_node_left);   //把此點父節點的L
				}
				else {  // 此點是他父節點的右邊節點
					node->parent->set_rightchild(old_node_left); // 把此節點的父親指向下一層新孩子
				}
			}
			else {					// 此點是root
				old_node_left->parent = NULL;
				this->root = old_node_left;
			}
			old_node_left->set_rightchild(node);
			node->set_leftchild(old_node_left_right);
			return old_node_left;
		}
		void balance_tree(binary_treenode<TKey, TData>* node) {
			binary_treenode<TKey, TData>* balance_node = node;
			int prev_bf = 0x80000000; // 由下往上找節點  上層節點樹高差
			int nodeleft_height = -1;
			int noderight_height = -1;
			while (balance_node != NULL) {
				if (nodeleft_height == -1)			//get subtree height L R
					nodeleft_height = balance_node->have_leftchild() ? balance_node->leftchild->get_subtree_height() : 0;
				if (noderight_height == -1)
					noderight_height = balance_node->have_rightchild() ? balance_node->rightchild->get_subtree_height() : 0;
				int bf = nodeleft_height - noderight_height;
				if (bf == 2) {
					if(prev_bf == 0x80000000)
						prev_bf = calculate_balance_factor(balance_node->leftchild);
					if (prev_bf == -1) {
						rotate_right(balance_node->leftchild);
					}
					balance_node = rotate_left(balance_node);
					bf -= 2;
					nodeleft_height--;
					noderight_height++;
				}
				else if (bf == -2) {
					if (prev_bf == 0x80000000)
						prev_bf = calculate_balance_factor(balance_node->rightchild);
					if (prev_bf == 1) {
						rotate_left(balance_node->rightchild);
					}
					balance_node = rotate_right(balance_node);
					bf += 2;
					nodeleft_height++;
					noderight_height--;
				}
				if (balance_node->is_leftnode()) {    				//此節點為上層父節點的左邊
					nodeleft_height = max(nodeleft_height, noderight_height) + 1;
					noderight_height = -1;
				}
				else if (balance_node->is_rightnode()) {			//此節點為上層父節點的右邊
					noderight_height = max(nodeleft_height, noderight_height) + 1;
					nodeleft_height = -1;
				}
				prev_bf = bf;
				balance_node = balance_node->parent;
			}
		}
		int calculate_balance_factor(binary_treenode<TKey, TData>* node) {
			int l_h = node->have_leftchild() ? node->leftchild->get_subtree_height() : 0;
			int r_h = node->have_rightchild() ? node->rightchild->get_subtree_height() : 0;
			return l_h - r_h;
		}
	};
}

using namespace edu_stat;
using namespace tableutil;
using namespace tree_util;
using namespace timerutil;

int str_compare(const string& val1, const string& val2) {
	return val1.compare(val2);
}
int int_compare(const int& val1, const int& val2) {
	if (val1 < val2)
		return COMPARE_A_BEFORE_B;
	else if (val1 > val2)
		return COMPARE_B_BEFORE_A;
	else
		return COMPARE_A_EQUAL_B;
}






void Task_1() {
	int file_id;

	string file_name = string("");
	bool success = false, eof_token = false;
	do {
		printf("File ID: ");
		file_id = stdioutil::read_int32(success, eof_token);
		file_name = string("input") + stringutil::to_string(file_id) + string(".txt");
		if (success && !fileioutil::file_exists(file_name)) {
			printf("*** ERROR: File: %s not found. ***\n", file_name.data());
			success = false;
		}
		if (eof_token) {
			printf("User stop the operation.\n");
			return;
		}
	} while (!success);
	ifstream input_stream;
	string tmp;
	input_stream.open(file_name);
	getline(input_stream, tmp);
	getline(input_stream, tmp);




	table input_table = table();				//input_table object
	//all data
	input_table.read_from_stream(input_stream, '\t', true);
	input_stream.close();





	twothree_tree<string, school_graduation_info> tree = twothree_tree<string, school_graduation_info>(str_compare);
	for (int i = 0; i < input_table.get_row_count(); i++) {
		const table_row& cur_row = input_table[i];
		graduation_record record = graduation_record(i + 1, cur_row);    //把各資料賦值
		tree[record.school_name].push_record(record);  //1323return foundData     ==> returnData.push_record(record)  
	}


	twothree_tree<string, school_graduation_info>::twothree_treenode* root = tree.get_root();
	if (root == NULL) {
		throw runtime_error("There is no any readable data in given table.\nPlease check whether if this table is vaild for this task or not.");
	}
	int counter = 1;
	int tree_height = tree.get_tree_height();
	int tree_node_count = tree.get_node_count();
	printf("[ 2-3 Tree ]\n");
	printf("    Tree height: %3d\n    Tree node count: %3d\n    Root node data: \n", tree_height, tree_node_count);
	vector<int> read_pos = vector<int>(root->get_data_count(), 0);
	vector<vector<graduation_record>> nodedata;
	for (int i = 0; i < root->get_data_count(); i++) {
		nodedata.push_back(root->data[i].data.records);
	}
	bool read_end = false;
	while (!read_end) {
		read_end = true;
		int min = 0;
		for (int i = 0; i < root->get_data_count(); i++) {
			if (read_pos[i] < nodedata[i].size()) {
				read_end = false;
				if (read_pos[min] >= nodedata[min].size() || nodedata[i][read_pos[i]].record_id < nodedata[min][read_pos[min]].record_id) {
					min = i;
				}
			}
		}
		if (!read_end) {
			const graduation_record& record = nodedata[min][read_pos[min]];
			printf("    %4d:  [%3d] %s\t%s\t%s\t%s\t%d\n",
				counter++,
				record.record_id,
				record.school_name.c_str(),
				record.department_name.c_str(),
				record.department_type.c_str(),
				record.level.c_str(),
				record.graduation_count);
			read_pos[min]++;
		}
	}
	
}




void Task_2() {
	int file_id;
	string file_name = string("");
	bool success = false, eof_token = false;
	do {
		printf("File ID: ");
		file_id = stdioutil::read_int32(success, eof_token);
		file_name = string("input") + stringutil::to_string(file_id) + string(".txt");

							// 檢查檔案室否存在
		if (success && !fileioutil::file_exists(file_name)) {
			printf("*** ERROR: File: %s not found. ***\n", file_name.data());
			success = false;
		}

		//ctrl+z
		if (eof_token) {
			printf("User stop the operation.\n");
			return;
		}
	} while (!success);

	ifstream input_stream;
	string tmp;
	input_stream.open(file_name);
	getline(input_stream, tmp);
	getline(input_stream, tmp);
	table input_table = table();
	input_table.read_from_stream(input_stream, '\t', true);
	input_stream.close();



	avl_tree<int, vector<graduation_record>> avltree = avl_tree<int, vector<graduation_record>>(int_compare);
	
	for (int i = 0; i < input_table.get_row_count(); i++) {
		const table_row& cur_row = input_table[i];
		graduation_record record = graduation_record(i + 1, cur_row);
		avltree[record.graduation_count].push_back(record);
	}


	binary_treenode<int, vector<graduation_record>>* root = avltree.get_root();
	if (root == NULL) {
		throw runtime_error("There is no any readable data in given table.\nPlease check whether if this table is vaild for this task or not.");
	}


	int tree_height = avltree.get_tree_height();
	int tree_node_count = avltree.get_node_count();
	printf("[ AVL Tree ]\n");
	printf("    Tree height: %3d\n    Tree node count: %3d\n    Root node data: \n", tree_height, tree_node_count);
	int counter = 1;


	 
	const vector<graduation_record>& school_info = root->nodedata.data;
	for (int j = 0; j < school_info.size(); j++) {
		const graduation_record& record = school_info[j];
		printf("    %4d:  [%3d] %s\t%s\t%s\t%s\t%d\n",
			counter++,
			record.record_id,
			record.school_name.c_str(),
			record.department_name.c_str(),
			record.department_type.c_str(),
			record.level.c_str(),
			record.graduation_count);
	}
}



void Task_3() {
	
}




void print_usage() {
	printf("-----------------------------------------------------------------------------------\n");
	printf("Usage:");
	printf("\t1) Create a 2-3 tree by school name.\n");
	printf("\t2) Create a avl tree by graduation count.\n");
	printf("\t0) Exit.\n");
	printf("\tIf you press Ctrl + Z while task ask you to input something, \n\tThis operation will stop current task.\n");
	printf("-----------------------------------------------------------------------------------\n");
}



int main() {
	bool success = false;
	bool exit_flag = false;
	bool eof_token = false;
	while (!exit_flag) {
		int select_task = -1;
		do {
			print_usage();
			printf("Select Task: ");
			select_task = stdioutil::read_int32(success, eof_token);
			if (eof_token) {
				exit_flag = true;
				break;
			}
		} while (!success);


		timer task_timer = timer();
		task_timer.start();
		switch (select_task) {
		case 0:
			exit_flag = true;
			break;
		case 1:
			try {
				printf(">>>>>>>>>>>>  Task 1 Start  <<<<<<<<<<<<\n");
				Task_1();
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
				Task_2();
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
				Task_3();
				printf("<<<<<<<<<<<<  Task 3 End    >>>>>>>>>>>>\n");
			}
			catch (exception& ex) {
				fprintf(stderr, "EXCEPTION: %s\n", ex.what());
				printf("!!!!!!!!!!!!  Task 3 Abort  !!!!!!!!!!!!\n\n");
			}
			break;
		default:
			printf("*** ERROR: Unknown task: %d ***\n", select_task);
			break;
		}
		task_timer.stop();
		double task_spends = task_timer.get_duration();
		printf("> Task spends: %.3lf ms\n", task_spends);
	}
	return 0;
}
