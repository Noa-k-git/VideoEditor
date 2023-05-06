#include "ServerProtocol.h"

std::tuple<bool, std::string, bool, std::string> server_protocol::ParseResponse(const std::string& data)
{
	std::string cmd;
	bool status = false;
	bool succeed = true;

	std::string header = data.substr(0, MSG_HEADER_LENGTH), message = data.substr(MSG_HEADER_LENGTH);
	std::vector<std::string> header_fields = SplitString(header, DELIMITER);
	try {
		cmd = header_fields.at(0);
		int length = std::stoi(header_fields.at(1));
		if (length != message.size()) {
			succeed = false;
			message = "Message corrupted";
		}
		status = CMD_STATUS.at(header_fields.at(2));
	}
	catch (const std::exception& e) {
		succeed = false;
		message = e.what();
	}
	
	return std::tuple<bool, std::string, bool, std::string>(succeed, cmd.substr(0, cmd.find_last_not_of(' ')), status, message);
}


std::vector<std::string> server_protocol::ParseMessage(const std::string& message)
{
	for (auto separator : SEPARATORS) {
		if (message.find(separator) != std::string::npos) {
			std::vector<std::string> parts = SplitString(message, separator);
			return parts;
		}
	}
	return std::vector<std::string>();
}

std::string server_protocol::BuildMessage(const std::vector<std::string>& msgLst)
{
	for (int i = sizeof(SEPARATORS) / sizeof(char) - 2; i > -1 ; i--) {
		for (auto msg : msgLst) {
			ReplaceChar(msg, SEPARATORS[i], SEPARATORS[i+1]);
		}
	}
	
	return JoinString(msgLst, SEPARATORS[0]);
}

std::string server_protocol::PadField(const std::string& field, const int length, const char ch)
{
	std::string buffer(length - field.size(), ch);
	if (ch == '0')
		return buffer + field;
	return field + buffer;
}

std::vector<std::string> server_protocol::BuildRequest(const std::string& cmd, const std::string& message)
{
	std::vector<std::string> requestElm;
	requestElm.push_back(PadField(cmd, CMD_FIELD_LENGTH, ' '));
	int length = message.length();
	if (length > MAX_DATA_LENGTH) {
		requestElm.clear();
		return requestElm;
	}
	requestElm.push_back(PadField(std::to_string(length), SIZE_FIELD_LENGTH));
	requestElm.push_back(message);
	std::string fullMsg = JoinRequestFields(requestElm);
	std::vector<std::string> parts;
	int counter = 1;
	while (fullMsg != "") {
		int slicer = fmin(fullMsg.length(), PART_SIZE - NUM_FIELD_LENGTH);
		parts.push_back(PadField(std::to_string(counter), NUM_FIELD_LENGTH, '0') + fullMsg.substr(0, slicer));
		fullMsg = fullMsg.substr(slicer);
		counter++;
	}
	parts.push_back(PadField(std::to_string(counter), NUM_FIELD_LENGTH, '0'));
	return parts;
}

std::string server_protocol::JoinRequestFields(const std::vector<std::string>& fields)
{
	return JoinString(fields, DELIMITER);
}

std::vector<std::string> server_protocol::SplitString(std::string str, char ch)
{
	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;
	while ((pos = str.find(ch)) != std::string::npos) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + 1);
	}
	result.push_back(str);
	return result;
}

