#pragma once
#include "Client.h"
#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/app.h>
#include <thread>
#include <chrono>
#ifndef PROTOCOL_FUNC
#define PROTOCOL_FUNC
#define PARSE_RESPONSE(data) server_protocol::ParseResponse(data, privateKey, myN)
#define BUILD_REQUEST(cmd, msg) server_protocol::BuildRequest(cmd, msg, serverKey, serverN)
#endif
ServerClient::ServerClient()
{
    listeningSocket = INVALID_SOCKET;
    writeSocket = INVALID_SOCKET;
    userId = INVALID_USER_ID;
    privateKey = 0, publicKey = 0, serverKey = 0, myN = 0, serverN = 0;
    rsa_cipher::setkeys(privateKey, publicKey, myN);
    WSADATA wsaData;

    // Initialize Winsock
    // MAKEWORD(2,2) parameter specifies the version of Winsock that the program is requestin to use
    // version 2.2

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        wxMessageBox(wxT("WSAStartup failed: " + std::to_string(iResult)), wxT("FATAL ERROR"), wxICON_ERROR);
        exit(EXIT_FAILURE);
    }
    CreateConnection();
}

ServerClient::~ServerClient()
{
    // Cleanup
    closesocket(writeSocket);
    WSACleanup();
}

void ServerClient::CreateConnection()
{
    Connect(writeSocket);
    if (writeSocket == INVALID_SOCKET)
    {
        wxLocale local(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);
        wxMessageBox(wxT("Could not connect to server"), wxT("FATAL ERROR"), wxICON_ERROR);
        //wxMessageDialog dialog(nullptr, "Could not connect to server", wxT("FATAL ERROR"), wxICON_ERROR);
        //dialog.ShowModal();
        exit(EXIT_FAILURE);
    }
    std::vector<std::string> requestParts;
    server_protocol::StringToParts((std::string)"SECURITY", requestParts);
    SendParts(requestParts);
    std::string data= RecieveMessage(writeSocket);
    std::vector<std::string> keyN = server_protocol::SplitString(data, '|');
    serverKey = std::stoi(keyN.at(0));
    serverN = std::stoi(keyN.at(1));
    SendKeys();
    //int iResult;

    // Send and receive data
    //char sendbuf[] = "Hello from the client!";
    //iResult = send(writeSocket, sendbuf, strlen(sendbuf), 0);
    //if (iResult == SOCKET_ERROR) {
    //    std::cout << "Error sending data: " << WSAGetLastError() << std::endl;
    //    closesocket(writeSocket);
    //    WSACleanup();
    //    return;
    //}

    //char recvbuf[256];
    //iResult = recv(writeSocket, recvbuf, sizeof(recvbuf), 0);


    return;
}

void ServerClient::SendParts(const std::vector<std::string>& requestParts)
{
    int res = 0;
    for (auto& part : requestParts) {
        res = send(writeSocket, part.c_str(), part.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (res == SOCKET_ERROR)
        {
            CreateConnection();
        }
    }
}

std::tuple<bool, std::string> ServerClient::SendRecieve(std::string cmd, std::string message)
{
    std::tuple<bool, std::string, bool, std::string> info;
    do {
        std::vector<std::string> requestParts = BUILD_REQUEST(cmd, message);
        SendParts(requestParts);
        std::string data = RecieveMessage(writeSocket);
        info = PARSE_RESPONSE(data);
    } while (!std::get<0>(info));
    return std::tuple<bool, std::string>(std::get<2>(info), std::get<3>(info));
}

void ServerClient::SendKeys()
{
    std::string msg = server_protocol::BuildMessage({ std::to_string(publicKey), std::to_string(myN) });
    std::tuple<bool, std::string> info;
    do { info = SendRecieve("RSAKEY", msg); } while (!std::get<0>(info));
}

std::tuple<bool, std::string> ServerClient::Signup(const std::string& username, const std::string& email, std::string password)
{
    std::tuple<bool, std::string> info;
    EncryptPassword(password);
    std::string msg = server_protocol::BuildMessage({ username, email, password });
    info = SendRecieve("SIGNUP", msg);
    if (std::get<0>(info)) {
        userId = std::stoi(std::get<1>(info));
    }
    return info;
}

std::tuple<bool, std::string> ServerClient::Login(const std::string& mail, std::string password)
{
    std::tuple<bool, std::string> info;
    EncryptPassword(password);
    std::string msg = server_protocol::BuildMessage({ mail, password });
    info = SendRecieve("LOGIN", msg);

    if (std::get<0>(info)) {
        userId = std::stoi(std::get<1>(info));
    }
    return info;
}

void ServerClient::Logout()
{
    SendRecieve("LOGOUT", "");
    userId = INVALID_USER_ID;
}

void ServerClient::PullInfo()
{
}

bool ServerClient::IsValidId()
{
    return this->userId != INVALID_USER_ID;
}


void ServerClient::Connect(SOCKET& sock)
{
    // Create a socket
   // AV_INET -> IPv4
   // SOCK_STREAM -> a stream socket that provides a reliable, sequenced, and unduplicated flow of data with no record boundaries
   // IPPROTO_TCP -> TCP protocol
    int iResult;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cout << "Error creating socket: " << WSAGetLastError() << std::endl;
        sock = INVALID_SOCKET;
        return;
    }

    // Connect to server
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(SERVER_PORT); // Port number of the server
    int result = inet_pton(AF_INET, SERVER_IP, &addr.sin_addr); // converts the ip address to binary format
    if (result != 1) {
        std::cout << "Error converting IP address" << std::endl;
        sock = INVALID_SOCKET;
        return;
    }

    iResult = connect(sock, (sockaddr*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        sock = INVALID_SOCKET;
        return;
    }
}

void ServerClient::Listener()
{
    Connect(listeningSocket);
    std::string data = RecieveMessage(listeningSocket);
    auto info = server_protocol::ParseResponse(data, privateKey, myN);
    bool succeed = std::get<0>(info);
    if (succeed)
    {
        std::string cmd = std::get<1>(info);
        std::string message = std::get<3>(info);
        // Handle cmd
    }

}

std::string ServerClient::RecieveMessage(SOCKET& sock)
{
    char recvbuf[server_protocol::PART_SIZE];
    std::vector<std::tuple<int, std::string>> parts;
    std::string fullMsg = "";
    while (true) {
        std::memset(recvbuf, 0, sizeof(recvbuf));
        int iResult = recv(sock, recvbuf, sizeof(recvbuf), 0);
        /*if (iResult > 0) {
            std::cout << "Received: " << recvbuf << std::endl;
        }
        else if (iResult == 0) {
            std::cout << "Connection closed" << std::endl;
        }
        else {
            std::cout << "Error receiving data: " << WSAGetLastError() << std::endl;
        }*/
        std::string part(recvbuf);
        if (part.length() == server_protocol::NUM_FIELD_LENGTH)
            break;
        int num = std::stoi(part.substr(0, server_protocol::NUM_FIELD_LENGTH));
        parts.push_back(std::make_tuple(num, part.substr(server_protocol::NUM_FIELD_LENGTH)));
    }
    std::sort(parts.begin(), parts.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    for (auto pair_ : parts) {
        fullMsg += std::get<1>(pair_);
    }
    
    return shift_cipher::dectypt(fullMsg, server_protocol::SHIFT_KEY);;
}

void ServerClient::EncryptPassword(std::string& password)
{
    char* password_c = new char[password.size() + 1];
    std::strcpy(password_c, password.c_str());
    password = SHA256(password_c);

    delete[] password_c;
}
