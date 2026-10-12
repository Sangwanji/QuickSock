#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

SOCKET sock;

DWORD WINAPI receiveMessages(LPVOID param) {  
    char buffer[4096];
    while (true) {
        int bytesReceived = recv(sock, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "Disconnected from server.\n";
            break;
        }
        std::cout << std::string(buffer, bytesReceived) << std::endl;
    }
    return 0;
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Connection failed. Is the server running?\n";
        WSACleanup();
        return 1;
    }

    std::cout << "Connected! Enter your chat name: ";
    std::string name;
    std::getline(std::cin, name);

    
    std::string joinMsg = "*** " + name + " joined the chat ***";
    send(sock, joinMsg.c_str(), (int)joinMsg.size(), 0);

    
    HANDLE hThread = CreateThread(nullptr, 0, receiveMessages, nullptr, 0, nullptr);
    CloseHandle(hThread);

    
    std::string message;
    while (std::getline(std::cin, message)) {
        if (message == "/quit") {
            std::string quitMsg = "*** " + name + " left the chat ***";
            send(sock, quitMsg.c_str(), (int)quitMsg.size(), 0);
            break;
        }

        if (message.empty()) continue;  

        std::string fullMessage = name + ": " + message;
        send(sock, fullMessage.c_str(), (int)fullMessage.size(), 0);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}