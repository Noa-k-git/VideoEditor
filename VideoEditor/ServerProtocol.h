#pragma once
#include <math.h>
#include <map>
#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include "rsa.h"
#include "ShiftCipher.h"

#ifndef SERVER_CONSTATNS
#define SERVER_CONSTATNS

#define SERVER_PORT 8989
#define SERVER_IP "127.0.0.1"
#endif

namespace server_protocol{
	const int NUM_FIELD_LENGTH = 8;
	const int CMD_FIELD_LENGTH = 16;
	const int SIZE_FIELD_LENGTH = 8;
	const int MAX_DATA_LENGTH = pow(10, SIZE_FIELD_LENGTH) - 1;
	const int MSG_HEADER_LENGTH = CMD_FIELD_LENGTH + 1 + SIZE_FIELD_LENGTH + 1;//  + STATUS_FIELD_LENGTH + 1 ; // Exact size of header(CMD + LENGTH fields);
	const int MAX_MSG_LENGTH = MSG_HEADER_LENGTH + MAX_DATA_LENGTH; // Max size of total message
	const int PART_SIZE = 2048;

	const char DELIMITER = '|';
	// Separators between args :
	//  1) ;    (x, x, x)-- > x; x; x
	//  2) * ((x, x), (x, x), (x, x))-- > x * x; x * x; x * x
	//  3) > (y, (x, x, (z, z, z)), y)-- > y; x * x > z > z > z; y

	const int SHIFT_KEY = 2;
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

	/**
	* @brief Receivs data, parses the data to command, code and message
	* 
	* @param data The data from the user
	* 
	* @return A tuple containing four values:
	* @return - succeed Whether the function run without errors.
	* @return - cmd The command in the header
	* @return - status The command status
	* @return - message The message
	* 
	*/
	std::tuple<bool, std::string, bool, std::string> ParseResponse(const std::string& data, int privateKey, int n);

	/**
	* @brief Parses the message part of a request from the user and returns a list of arguments passed by the user.
	* 
	* @param message The message part of a request from the user.
	* 
	* @return std::vectorstd::string The args in the request in a form of a vector.
	*/
	std::vector<std::string> ParseMessage(const std::string& message);
	
	void StringToParts(std::string fullMsg, std::vector<std::string>& res);

	/**
	 * @brief Builds a message string to send according to the protocol, using a vector of arguments
	 *
	 * @param msgLst A vector of strings representing the arguments for the message
	 *
	 * @return The resulting message string
	 */
	std::string BuildMessage(const std::vector<std::string>& msgLst);

	/**
	* @brief Pads a field to the wanted length with the char specified
	* 
	* @param field The field content
	* @param length The field wanted length
	* @param ch Pad a string with the char. If 0 will pad from the left, otherwise from the right. Defaults to '0'.
	* 
	* @return The buffered field
	*/
	std::string PadField(const std::string& field, const int length, const char ch = '0');

	/**
	 * @brief Build a request parts list to send to the server.
	 * @brief Receives command name and a message and builds a list for the request message from the client, according to  list of all the parts to be sent is returned.
	 *
	 * @param cmd The command name
	 * @param message The message to send to the server
	 *
	 * @return A list of all the parts to be sent to the server as the client request
	 *         If an error occurred, empty vector is returned.
	 */
	std::vector<std::string> BuildRequest(const std::string& cmd, const std::string& message, int serverKey, int serverN);
	std::string CreateHeader(const std::vector<std::string>& headerFields, int publicKey, int n);

	/*
	* @brief Helper method. Gets a list, joins all of it's fields to one string divided by the delimiter
	* 
	* @param fields The list to join to one string
	* 
	* @returns the message that looks like field1|field2|...
	
	*/
	std::string JoinRequestFields(const std::vector<std::string>& fields);

	/**
	* @brief Splits the given string by a char
	* @param str String to split
	* @param ch The char to split by
	* @return A vector of substrings
	*/
	std::vector<std::string> SplitString(std::string str, char ch);

	/**
	* @brief Replace all occurrences of char in a given string with another one.
	* @param str The string
	* @param old_ The char to replace
	* @param new_ the char replacing the old_ char
	* @return true if replaced any, false otherwise
	*/
	inline bool ReplaceChar(std::string& str, char old_, char new_) {

		if (str.find(old_) == std::string::npos)
			return false;

		std::replace(str.begin(), str.end(), old_, new_);
		return true;

	}

	/**
	* @brief Join a vector of strings by a char
	* @param elements The vecotr of strings
	* @param delimiter The delimiter between the strings
	* @return The join string
	*/
	inline std::string JoinString(const std::vector<std::string>& elements, char delimiter) {
		if (elements.empty()) {
			return "";
		}

		std::string result = elements[0];
		for (size_t i = 1; i < elements.size(); ++i) {
			result += delimiter;
			result += elements[i];
		}

		return result;
	}
}

