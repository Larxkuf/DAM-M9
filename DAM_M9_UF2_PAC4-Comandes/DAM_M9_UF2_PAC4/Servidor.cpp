// Servidor.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

std::mutex fileMutex;
int orderCounter = 1;

void handleClient(SOCKET clientSocket) {
    char buffer[1024] = { 0 };
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesReceived > 0) {
        std::string clientOrder(buffer);
        std::string orderId;

        {
            std::lock_guard<std::mutex> lock(fileMutex);
            orderId = "ORD-" + std::to_string(orderCounter++);
            std::ofstream outFile("comandes.txt", std::ios::app);
            outFile << orderId << ": \"" << clientOrder << "\"" << std::endl;
        }

        send(clientSocket, orderId.c_str(), orderId.size(), 0);
        std::cout << "Comanda rebuda: \"" << clientOrder << "\", Identificador assignat: " << orderId << std::endl;
    }

    closesocket(clientSocket);
}

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9100);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Servei iniciat. Escoltant al port 9100..." << std::endl;

    while (true) {
        sockaddr_in clientAddr{};
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);

        std::thread clientThread(handleClient, clientSocket);
        clientThread.detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
