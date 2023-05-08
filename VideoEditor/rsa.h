#ifndef H__RSA
#define H__RSA
#include <vector>
#include <string>
namespace rsa_encryption {
	void setkeys(int& private_key, int& public_key, int& n);
	std::vector<int> encoder(std::string message, const int& public_key, const int& n);
	std::string decoder(std::vector<int> encoded, const int& private_key, const int& n);
}
#endif