#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include "ServerProtocol.h"
#include "SHA256.h"
#include "rsa.h"
#include "ShiftCipher.h"
#ifndef CLIENT_CONSTANTS
#define CLIENT_CONSTANTS
#define INVALID_USER_ID -1
#endif
class ServerClient {
public:
	ServerClient();
	~ServerClient();
	// @brief Creates a connection with the server and return the connected socket
	void CreateConnection();
	void SendParts(const std::vector<std::string>& requestParts);
	std::tuple<bool, std::string> SendRecieve(std::string, std::string);
	void SendKeys();
	/// <summary>
	/// Signup request to the server
	/// </summary>
	/// <param name="username">User's username</param>
	/// <param name="email">User's mail</param>
	/// <param name="password">User's password, unencrypted</param>
	std::tuple<bool, std::string> Signup(const std::string& username, const std::string& email, std::string password);

	/// <summary>
	/// Login request to the server
	/// </summary>
	/// <param name="mail">The user's mail</param>
	/// <param name="password">The user's password, unencrypted</param>
	/// <returns> If Login succeed </returns>
	std::tuple<bool, std::string> Login(const std::string& mail, std::string password);

	/// <summary>
	/// Logout from the server, thus logout from the project
	/// </summary>
	void Logout();

	std::tuple<bool, std::string> PullInfo();
	void PushProject();
	std::tuple<bool, std::string> PullProject(std::string);
	bool IsValidId();
	inline std::string GetProjId() { return projId; }
	inline void SetProjId(std::string id_) { projId = id_; }
	inline std::string GetPrName() { return projectName; }
	inline void SetPrName(std::string name) { projectName = name; }
	inline std::string GetPath() { return projectPath; }
	inline void SetPath(std::string projPath) { projectPath = projPath; }
private:
	SOCKET listeningSocket;
	SOCKET writeSocket;

	int userId;
	std::string projId;
	std::string projectName;
	std::string projectPath;
	int publicKey;
	int privateKey;
	int myN;
	int serverKey;
	int serverN;

	void Connect(SOCKET &sock);
	void Listener();

	/// <summary>
	/// Recieves the full message from a given socket
	/// </summary>
	/// <param name="sock">A given socket</param>
	/// <returns>The full message</returns>
	std::string RecieveMessage(SOCKET& sock);
	void EncryptPassword(std::string& password);
};

//
//int main() {
//	// Generate public and private keys
//	mpuint d(32), e(32), n(32);
//	GenerateKeys(d, e, n);
//
//	// Convert string to numerical representation
//	std::string message = "This should be encrypted";
//	mpuint m(message.length() * 8);
//	for (int i = 0; i < message.length(); i++) {
//		m.value[i] = message[i];
//	}
//
//	// Encrypt message using public key
//	mpuint c(m.length);
//	EncryptDecrypt(c, m, e, n);
//
//	// Decrypt message using private key
//	mpuint decrypted(m.length);
//	EncryptDecrypt(decrypted, c, d, n);
//
//	// Convert decrypted message back to string
//	std::string decryptedMessage;
//	for (int i = 0; i < decrypted.length; i++) {
//		decryptedMessage += decrypted.value[i];
//	}
//
//	std::cout << "Original message: " << message << std::endl;
//	std::cout << "Decrypted message: " << decryptedMessage << std::endl;
//
//	return 0;
//}