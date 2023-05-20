#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace string_utils {
	/**
	* @brief Splits the given string by a char
	* @param str String to split
	* @param ch The char to split by
	* @return A vector of substrings
	*/
	std::vector<std::string> SplitString(std::string str, char ch, int max = -1);
	std::vector<std::string> SplitString(std::string str, std::string s, int max = -1);

	/**
	* @brief Finds the difference between string a and string b and returns it
	*/
	std::string FindDifference(const std::string& a, const std::string& b);
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