#pragma once
#include <string>

namespace shift_cipher {
	// This function receives text and shift and
	// returns the encrypted text
	std::string encrypt(std::string text, int s);
	// This function receives text and shift backwords and
	// returns the decrypted text
	std::string dectypt(std::string text, int s);
}