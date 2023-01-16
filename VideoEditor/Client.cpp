#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "Client.h"
#define PORT 38745

int CreateConnection() {
    WSADATA wsaData;
    int iResult;

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        std::cout << "WSAStartup failed: " << iResult << std::endl;
        return 1;
    }

    // Create a socket
    SOCKET ConnectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ConnectSocket == INVALID_SOCKET) {
        std::cout << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    // Connect to server
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT); // Port number of the server
    int result = inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    if (result == 1) {
        // Success
    }
    else {
        std::cout << "Error converting IP address"<< std::endl;
        return 1;
    }

    iResult = connect(ConnectSocket, (sockaddr*)&addr, sizeof(addr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Send and receive data
    char sendbuf[] = "Hello from the client!";
    iResult = send(ConnectSocket, sendbuf, strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        std::cout << "Error sending data: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    char recvbuf[256];
    iResult = recv(ConnectSocket, recvbuf, sizeof(recvbuf), 0);
    if (iResult > 0) {
        std::cout << "Received: " << recvbuf << std::endl;
    }
    else if (iResult == 0) {
        std::cout << "Connection closed" << std::endl;
    }
    else {
        std::cout << "Error receiving data: " << WSAGetLastError() << std::endl;
    }

    // Cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    return 0;
}
