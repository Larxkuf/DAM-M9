#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <ctime>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 9000

int main() {
    WSADATA wsaData;
    SOCKET serverSocket, clientSocket;
    sockaddr_in serverAddr, clientAddr;
    int addrLen = sizeof(clientAddr);

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Hi ha hagut un petitet error iniciant el Winsock" << std::endl;
        return -1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creant el socket del servidor :(" << std::endl;
        WSACleanup();
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Hi ha agur un error fent el bind al port, perque?" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Connexions escoltant les error hi ha hagut" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Wiiii, el servei ha iniciat. Escoltant al port: " << PORT << "..." << std::endl;

    while (true) {
        clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Sembla que ha donat error acceptant la connexió" << std::endl;
            continue;
        }

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);


        std::cout << "La connexió ha sigut acceptada des del client :DD" << clientIP << " >> ";


        std::time_t now = std::time(nullptr);
        std::tm timeInfo;
        if (localtime_s(&timeInfo, &now) != 0) {
            std::cerr << "Error obtenint l'hora local." << std::endl;
            closesocket(clientSocket);
            continue;
        }

        char buffer[100];
        std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", &timeInfo);


        send(clientSocket, buffer, strlen(buffer), 0);
        std::cout << "Hora i data enviades :\ : " << buffer << std::endl;

  
        closesocket(clientSocket);

        std::cout << "Esperando una nova connexió... 🎶🎶 (persona silbando)" << std::endl;
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
