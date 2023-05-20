#include "ServerProtocol.h"

std::tuple<bool, std::string, bool, std::string> server_protocol::ParseResponse(const std::string& data, int privateKey, int n)
{
	std::string cmd;
	bool status = false;
	bool succeed = true;

	size_t delimiterPos = data.find(DELIMITER);
	std::string encodedHheader = data.substr(0, delimiterPos), message = data.substr(delimiterPos+1);

	// decoding header
	std::vector<std::string> vecStrHeader = SplitString(encodedHheader, ',');
	std::vector<int> intEncodedHeader(vecStrHeader.size());
	std::transform(vecStrHeader.begin(), vecStrHeader.end(), intEncodedHeader.begin(), [](const std::string& str){
		return std::stoi(str);
		});

	std::string header = rsa_cipher::decoder(intEncodedHeader, privateKey, n);

	std::vector<std::string> header_fields = SplitString(header, DELIMITER);
	try {
		cmd = header_fields.at(0);
		status = CMD_STATUS.at(header_fields.at(1));
		int length = std::stoi(header_fields.at(2));
		if (length != message.size()) {
			succeed = false;
			message = "Message corrupted";
		}
	}
	catch (const std::exception& e) {
		succeed = false;
		message = e.what();
	}
	
	return std::tuple<bool, std::string, bool, std::string>(succeed, cmd.substr(0, cmd.find_last_not_of(' ') + 1), status, message);
}


std::vector<std::string> server_protocol::ParseMessage(const std::string& message, int max)
{
	std::vector<std::string> parts;
	for (auto separator : SEPARATORS) {
		if (message.find(separator) != std::string::npos) {
			if (max!= -1)
			parts = SplitString(message, separator, max);
			else
			parts = SplitString(message, separator);
			return parts;
		}
	}
	return {message};
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
void server_protocol::StringToParts(std::string fullMsg, std::vector<std::string>& res) {
	fullMsg = shift_cipher::encrypt(fullMsg, SHIFT_KEY);
	int counter = 1;
	while (fullMsg != "") {
		int slicer = fmin(fullMsg.length(), PART_SIZE - NUM_FIELD_LENGTH);
		res.push_back(PadField(std::to_string(counter), NUM_FIELD_LENGTH, '0') + fullMsg.substr(0, slicer));
		fullMsg = fullMsg.substr(slicer);
		counter++;
	}
	res.push_back(PadField(std::to_string(counter), NUM_FIELD_LENGTH, '0'));

}
std::vector<std::string> server_protocol::BuildRequest(const std::string& cmd, const std::string& message, int serverKey, int serverN)
{
	std::vector<std::string> headerElm;
	headerElm.push_back(PadField(cmd, CMD_FIELD_LENGTH, ' '));
	int length = message.length();
	if (length > MAX_DATA_LENGTH) {
		headerElm.clear();
		return headerElm;
	}
	headerElm.push_back(PadField(std::to_string(length), SIZE_FIELD_LENGTH));
	std::vector<std::string> requestElm;
	requestElm.push_back(CreateHeader(headerElm, serverKey, serverN));
	requestElm.push_back(message);
	std::string fullMsg = JoinRequestFields(requestElm);
	std::vector<std::string> parts;
	StringToParts(fullMsg, parts);

	return parts;
}

std::string server_protocol::CreateHeader(const std::vector<std::string>& headerFields, int publicKey, int n) {
	std::string decoded = server_protocol::JoinRequestFields(headerFields);
	std::vector<int> encodedIntVec = rsa_cipher::encoder(decoded, publicKey, n);
	std::vector<std::string> encodedStrVec (encodedIntVec.size());
	std::transform(encodedIntVec.begin(), encodedIntVec.end(), encodedStrVec.begin(), [](const int& i) {
		return std::to_string(i);
		});
	return JoinString(encodedStrVec, ',');
}
std::string server_protocol::JoinRequestFields(const std::vector<std::string>& fields)
{
	return JoinString(fields, DELIMITER);
}


