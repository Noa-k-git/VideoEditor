#include "string_utils.h"
std::vector<std::string> string_utils::SplitString(std::string str, char ch, int max) {

	std::vector<std::string> result;
	size_t pos = 0;
	std::string token;

	int counter = 0;
	while ((pos = str.find(ch)) != std::string::npos && (max == -1 || counter < max)) {
		token = str.substr(0, pos);
		result.push_back(token);
		str.erase(0, pos + 1);
		counter++;
	}

	// Add the remaining part of the string as the last element if it exists
	if (!str.empty()) {
		result.push_back(str);
	}
	return result;

}

std::vector<std::string> string_utils::SplitString(std::string str, std::string delimiter, int max) {

	std::vector<std::string> result;
	size_t pos = 0;
	size_t delimiterLength = delimiter.length();

	while (pos < str.length()) {
		size_t found = str.find(delimiter, pos);

		if (found == std::string::npos) {
			// Add the remaining part of the string as the last element
			result.push_back(str.substr(pos));
			break;
		}

		// Add the substring before the delimiter
		result.push_back(str.substr(pos, found - pos));

		// Move the position past the delimiter
		pos = found + delimiterLength;
	}
	return result;

}

std::string string_utils::FindDifference(const std::string& a, const std::string& b)
{
	auto mismatchResult = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
	std::string difference = mismatchResult.second == b.end() ? "" : std::string(mismatchResult.second, b.end());
	return difference;
}
