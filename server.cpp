#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "ws2_32.lib")

std::vector<SOCKET> clients;
CRITICAL_SECTION clientCS;  

void broadcast(const std::string& message, SOCKET sender) {
    EnterCriticalSection(&clientCS);
    for (SOCKET client : clients) {
        if (client != sender)
            send(client, message.c_str(), (int)message.size(), 0);
    }
    LeaveCriticalSection(&clientCS);
}

DWORD WINAPI handleClient(LPVOID param) {  
    SOCKET clientSocket = (SOCKET)param;
    char buffer[4096];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            std::cout << "Client disconnected.\n";
            break;
        }
        std::string message(buffer, bytesReceived);
        std::cout << message << std::endl;
        broadcast(message, clientSocket);
    }

    EnterCriticalSection(&clientCS);
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    LeaveCriticalSection(&clientCS);
    closesocket(clientSocket);
    return 0;
}

int main() {
    InitializeCriticalSection(&clientCS);  

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Server listening on port 54000...\n";

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);
        if (clientSocket == INVALID_SOCKET) continue;

        EnterCriticalSection(&clientCS);
        clients.push_back(clientSocket);
        LeaveCriticalSection(&clientCS);

        std::cout << "New client connected!\n";

        
        HANDLE hThread = CreateThread(nullptr, 0, handleClient, (LPVOID)clientSocket, 0, nullptr);
        CloseHandle(hThread);
    }

    closesocket(serverSocket);
    WSACleanup();
    DeleteCriticalSection(&clientCS);
    return 0;
}