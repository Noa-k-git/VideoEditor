#pragma once
#include "Client.h"

#ifndef CLIENT_MACROS
#define CLIENT_MACROS
#define PARSE_RESPONSE(data) server_protocol::ParseResponse(data, privateKey, myN)
#define BUILD_REQUEST(cmd, msg) server_protocol::BuildRequest(cmd, msg, serverKey, serverN)
#define W_SEND_RECEIVE(cmd, msg) SendRecieve(writeSocket, cmd, msg)
#define L_SEND_RECEIVE(cmd, msg) SendRecieve(listenSocket, cmd, msg)
#endif

wxDEFINE_EVENT(SWAP_CLIP_SERVER_EVT, wxCommandEvent);

ServerClient::ServerClient(wxWindow* swWindow)
{
    swapWindow = swWindow;
    listenSocket = INVALID_SOCKET;
    writeSocket = INVALID_SOCKET;
    userId = INVALID_USER_ID;
    projectPath = "";
    projectName = "";
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
    SendParts(writeSocket, requestParts);
    std::string data= RecieveMessage(writeSocket);
    std::vector<std::string> keyN = SplitString(data, '|');
    serverKey = std::stoi(keyN.at(0));
    serverN = std::stoi(keyN.at(1));
    SendKeys();
    std::thread listen(&ServerClient::Listener, this);
    listen.detach();
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

void ServerClient::SendParts(SOCKET& sock, const std::vector<std::string>& requestParts)
{
    int res = 0;
    for (auto& part : requestParts) {
        res = send(sock, part.c_str(), part.size(), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (res == SOCKET_ERROR)
        {
            CreateConnection();
        }
    }
}

std::tuple<bool, std::string> ServerClient::SendRecieve(SOCKET& sock, std::string cmd, std::string message)
{
    std::tuple<bool, std::string, bool, std::string> info;
    do {
        std::vector<std::string> requestParts = BUILD_REQUEST(cmd, message);
        SendParts(sock, requestParts);
        std::string data = RecieveMessage(sock);
        info = PARSE_RESPONSE(data);
    } while (!std::get<0>(info));
    return std::tuple<bool, std::string>(std::get<2>(info), std::get<3>(info));
}

void ServerClient::SendKeys()
{
    std::string msg = server_protocol::BuildMessage({ std::to_string(publicKey), std::to_string(myN) });
    std::tuple<bool, std::string> info;
    do { info = W_SEND_RECEIVE("RSAKEY", msg); } while (!std::get<0>(info));
}

std::tuple<bool, std::string> ServerClient::Signup(const std::string& username, const std::string& email, std::string password)
{
    std::tuple<bool, std::string> info;
    EncryptPassword(password);
    std::string msg = server_protocol::BuildMessage({ username, email, password });
    info = W_SEND_RECEIVE("SIGNUP", msg);
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
    info = W_SEND_RECEIVE("LOGIN", msg);

    if (std::get<0>(info)) {
        userId = std::stoi(std::get<1>(info));
    }
    return info;
}

std::tuple<bool, std::string> ServerClient::CreateProject(std::vector<std::string> elems)
{
    std::tuple<bool, std::string> info;
    info = W_SEND_RECEIVE("CREATE", server_protocol::BuildMessage(elems));
    return info;
}

std::tuple<bool, std::string> ServerClient::ConnectProject()
{
    std::tuple<bool, std::string> info;
    info = W_SEND_RECEIVE("CONNECTPROJ", projId);

    return info;
}

void ServerClient::Logout()
{
    W_SEND_RECEIVE("LOGOUT", "");
    userId = INVALID_USER_ID;
}

std::tuple<bool, std::string> ServerClient::PullInfo()
{
    std::tuple<bool, std::string> info;
    std::string msg = std::to_string(userId);
    info = W_SEND_RECEIVE("PULLINFO", msg);
    return info;
}

void ServerClient::PushProject()
{
    if (projectName != "" && projectPath != "")
    {
        std::string line;
        std::string project = "";
        // Read from the text file
        std::ifstream file(projectPath + projectName);

        // Use a while loop together with the getline() function to read the file line by line
        while (std::getline(file, line)) {
            project += line + '\n';
        }

        // Close the file
        file.close();
        W_SEND_RECEIVE("PUSHPROJECT", server_protocol::BuildMessage({ project }));
    }
}

std::tuple<bool, std::string> ServerClient::PullProject()
{
    std::tuple<bool, std::string> info;
    return W_SEND_RECEIVE("PULLPROJECT", "");
    
}

void ServerClient::SendUpdateProject(std::string data)
{
    W_SEND_RECEIVE("UPDATEPROJ", data);
}

void ServerClient::Switch(std::string seqName, int idx1, int idx2)
{
    std::string msg = server_protocol::BuildMessage({ "switch", seqName, std::to_string(idx1), std::to_string(idx2)});
    SendUpdateProject(msg);
}

void ServerClient::HandleUpdate(std::vector<std::string> updateParms)
{
    std::string cmd = updateParms.at(0);
    updateParms.erase(updateParms.begin());
    if (cmd == "switch")
    {
        if (updateParms.size() == 3)
        {
            wxCommandEvent swapEvt_(SWAP_CLIP_SERVER_EVT);
            swapEvt_.SetString(updateParms.at(0));
            swapEvt_.SetInt(std::stoi(updateParms.at(1)));
            swapEvt_.SetExtraLong(std::stoi(updateParms.at(2)));
            wxPostEvent(swapWindow, swapEvt_);
        }
    }
        
}

void ServerClient::Listener()
{
    if (listenSocket != INVALID_SOCKET)
        return;
    Connect(listenSocket);
    std::tuple<bool, std::string> tempInfo;
    do { tempInfo = L_SEND_RECEIVE("LISTEN", server_protocol::BuildMessage({ std::to_string(publicKey), std::to_string(myN) })); }
    while(!std::get<0>(tempInfo));

    if (listenSocket == INVALID_SOCKET)
    {
        wxLocale local(wxLANGUAGE_ENGLISH, wxLOCALE_DONT_LOAD_DEFAULT);
        wxMessageBox(wxT("Could not connect to server"), wxT("FATAL ERROR"), wxICON_ERROR);
        //wxMessageDialog dialog(nullptr, "Could not connect to server", wxT("FATAL ERROR"), wxICON_ERROR);
        //dialog.ShowModal();
        exit(EXIT_FAILURE);
    }
    std::tuple<bool, std::string, bool, std::string> info;
    while (listenSocket != INVALID_SOCKET) {
        std::string msg = std::to_string(userId);
        std::string data = RecieveMessage(listenSocket);
        info = PARSE_RESPONSE(data);
        bool succeed = std::get<0>(info);
        if (succeed)
        {
            std::string cmd = std::get<1>(info);
            if (cmd == "UPDATEPROJ") {
                std::vector<std::string> updateParms = server_protocol::ParseMessage(std::get<3>(info));
                if (updateParms.size() > 0)
                {
                    std::thread t(&ServerClient::HandleUpdate, this, updateParms);
                    t.detach();
                }
                //TODO: handle changes
            }
        }
    }
}

bool ServerClient::IsValidId()
{
    return this->userId != INVALID_USER_ID;
}

void ServerClient::SetProjId(std::string id_)
{
    projId = id_;
    ConnectProject();
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
