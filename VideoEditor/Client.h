#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <fstream>
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <thread>
#include <chrono>
#include "ServerProtocol.h"
#include "SHA256.h"
#include "rsa.h"
#include "ShiftCipher.h"
#include "App.h"
#ifndef CLIENT_CONSTANTS
#define CLIENT_CONSTANTS
#define INVALID_USER_ID -1
#endif

wxDECLARE_EVENT(SWAP_CLIP_SERVER_EVT, wxCommandEvent);

class ServerClient {
public:
	ServerClient();
	~ServerClient();
	void SetSwapWindow(wxWindow* win);
	// @brief Creates a connection with the server and return the connected socket
	void CreateConnection();
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
	
	std::tuple<bool, std::string> CreateProject(std::vector <std::string>);
	std::tuple<bool, std::string> ConnectProject();

	/// <summary>
	/// Logout from the server, thus logout from the project
	/// </summary>
	void Logout();

	std::tuple<bool, std::string> PullInfo();
	void PushProject();
	std::tuple<bool, std::string> PullProject();
	void SendUpdateProject(std::string data);

	void Switch(std::string seqName, int idx1, int idx2);
	void HandleUpdate(std::vector<std::string> updateParms);

	bool IsValidId();
	inline std::string GetProjId() { return projId; }
	void SetProjId(std::string id_);
	inline std::string GetPrName() { return projectName; }
	inline void SetPrName(std::string name) { projectName = name; }
	inline std::string GetPath() { return projectPath; }
	inline void SetPath(std::string projPath) { projectPath = projPath; }
private:
	SOCKET listenSocket;
	SOCKET writeSocket;

	wxWindow* swapWindow;
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
	void SendParts(SOCKET &sock, const std::vector<std::string>& requestParts);
	std::tuple<bool, std::string> SendRecieve(SOCKET &sock, std::string, std::string);

	/// <summary>
	/// Recieves the full message from a given socket
	/// </summary>
	/// <param name="sock">A given socket</param>
	/// <returns>The full message</returns>
	std::string RecieveMessage(SOCKET& sock);
	void EncryptPassword(std::string& password);
};