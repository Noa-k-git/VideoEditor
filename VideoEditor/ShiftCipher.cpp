#include "ShiftCypher.h"

std::string shift_cipher::encrypt(std::string text, int s)
{
	std::string result = "";

	// traverse text
	for (int i = 0; i < text.length(); i++) {
		// apply transformation to each character
		// Encrypt Uppercase letters

		result += char(text[i] + s);
	}

	// Return the resulting string
	return result;
}

std::string shift_cipher::dectypt(std::string text, int s)
{
	return shift_cipher::encrypt(text, -s);
}
