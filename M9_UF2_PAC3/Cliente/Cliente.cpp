#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

#define PORT 9000
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET clientSocket;
    sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Hi ha hagut un petitet error iniciant el Winsock" << std::endl;
        return -1;
    }

    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error creant el socket del client :(" << std::endl;
        WSACleanup();
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error connectant al servei, tot malament. Comprova si el servidor està actiu. >:(" << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    int bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
    if (bytesRead > 0) {
        std::cout << "Connexió establerta amb el servei. Yipi :D." << std::endl;
        std::cout << "Hora i data rebudes: " << buffer << std::endl;
    }
    else {
        std::cerr << "Error amb l'hora i data. No Rebudes" << std::endl;
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
