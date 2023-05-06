#include "Client.h"

Client::Client()
{
    listeningSocket = INVALID_SOCKET;
    writeSocket = INVALID_SOCKET;
    userId = INVALID_USER_ID;
    WSADATA wsaData;

    // Initialize Winsock
    // MAKEWORD(2,2) parameter specifies the version of Winsock that the program is requestin to use
    // version 2.2

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return;
    }
    CreateConnection();
}

Client::~Client()
{
    // Cleanup
    closesocket(writeSocket);
    WSACleanup();
}

void Client::CreateConnection()
{
    Connect(writeSocket);
    int iResult;

    // Send and receive data
    char sendbuf[] = "Hello from the client!";
    iResult = send(writeSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error sending data: " << WSAGetLastError() << std::endl;
        closesocket(writeSocket);
        WSACleanup();
        return;
    }

    char recvbuf[256];
    iResult = recv(writeSocket, recvbuf, sizeof(recvbuf), 0);


    return;
}

void Client::Signup(const std::string& username, const std::string& email, std::string password)
{
    int res = 0;
    std::tuple<bool, std::string, bool, std::string> info;
    EncryptPassword(password);
    
    do {
        std::string msg = server_protocol::BuildMessage({ username, email, password });
        std::vector<std::string> requestParts = server_protocol::BuildRequest("LOGIN", msg);
        for (auto& part : requestParts) {
            res = send(writeSocket, part.c_str(), part.size(), 0);
            if (res == SOCKET_ERROR)
            {
                // handle something
                return;
            }
        }
        std::string data = RecieveMessage(writeSocket);
        std::tuple<bool, std::string, bool, std::string> info = server_protocol::ParseResponse(data);
    } while (std::get<0>(info));
    if (std::get<2>(info)) {
        userId = std::stoi(std::get<3>(info));
    }
}

void Client::Login(const std::string& mail, std::string password)
{
    int res = 0;
    std::tuple<bool, std::string, bool, std::string> info;
    EncryptPassword(password);
    
    do {
        std::string msg = server_protocol::BuildMessage({ mail, password });
        std::vector<std::string> requestParts = server_protocol::BuildRequest("LOGIN", msg);
        for (auto& part : requestParts) {
            res = send(writeSocket, part.c_str(), part.size(), 0);
            if (res == SOCKET_ERROR)
            {
                // handle something
                return;
            }
        }
        std::string data = RecieveMessage(writeSocket);
        std::tuple<bool, std::string, bool, std::string> info = server_protocol::ParseResponse(data);
    } while (std::get<0>(info));
    if (std::get<2>(info)) {
        userId = std::stoi(std::get<3>(info));
    }
}

void Client::Logout()
{
}

void Client::PullInfo()
{
}

void Client::Connect(SOCKET& sock)
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

void Client::Listener()
{
    Connect(listeningSocket);
    std::string data = RecieveMessage(listeningSocket);
    auto info = server_protocol::ParseResponse(data);
    bool succeed = std::get<0>(info);
    if (succeed)
    {
        std::string cmd = std::get<1>(info);
        std::string message = std::get<3>(info);
        // Handle cmd
    }

}

std::string Client::RecieveMessage(SOCKET& sock)
{
    char recvbuf[server_protocol::PART_SIZE];
    std::vector<std::tuple<int, std::string>> parts;
    std::string fullMsg = "";
    while (true) {
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
    return fullMsg;
}

void Client::EncryptPassword(std::string& password)
{
    char* password_c = new char[password.size() + 1];
    std::strcpy(password_c, password.c_str());
    password = SHA256(password_c);

    delete[] password_c;
}
