#pragma once
#include <math.h>
#include <map>
#include <string>

namespace server_protocol{
	const int NUM_FIELD_LENGTH = 8;
	const int CMD_FIELD_LENGTH = 16;
	const int SIZE_FIELD_LENGTH = 8;
	const int MAX_DATA_LENGTH = pow(10, SIZE_FIELD_LENGTH) - 1;
	const int MSG_HEADER_LENGTH = CMD_FIELD_LENGTH + 1 + SIZE_FIELD_LENGTH + 1#  + STATUS_FIELD_LENGTH + 1 ; // Exact size of header(CMD + LENGTH fields);
	const int MAX_MSG_LENGTH = MSG_HEADER_LENGTH + MAX_DATA_LENGTH; // Max size of total message
	const int PART_SIZE = 2048;

	const char DELIMITER = '|';
	// Separators between args :
	//  1) ;    (x, x, x)-- > x; x; x
	//  2) * ((x, x), (x, x), (x, x))-- > x * x; x * x; x * x
	//  3) > (y, (x, x, (z, z, z)), y)-- > y; x * x > z > z > z; y
	const char SEPARATORS[3] = { ';', '*', '>' };
	const std::map < std::string, bool > CMD_STATUS = {
		{"OK", true},
		{"FA", false}
	};
	const std::string COMMANDS[9] = {
	"SIGNUP",
	"LOGIN",
	"LOGOUT",
	"PULLINFO",
	"CREATE",
	"ADDUSERS",
	"REMOVEUSER",
	"PUSHPROJECT",
	"PULLPROJECT"
	};

	static std::string parse_message(const std::string& message);
	static std::tuple<bool, std::string, std::string> parse_data(const std::string& data);
	static std::string build_message(const std::vector<std::string>& message_lst);
	static std::string pad_field(const std::string& field, size_t wanted_length, char ch = '0');
	static std::vector<std::string> build_response(const std::string& cmd, bool code, const std::string& message);
}

