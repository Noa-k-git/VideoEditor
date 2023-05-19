#include "string_utils.h"
std::vector<std::string> string_utils::SplitString(std::string str, char ch) {

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

std::string string_utils::FindDifference(const std::string& a, const std::string& b)
{
	auto mismatchResult = std::mismatch(a.begin(), a.end(), b.begin(), b.end());
	std::string difference = mismatchResult.second == b.end() ? "" : std::string(mismatchResult.second, b.end());
	return difference;
}
