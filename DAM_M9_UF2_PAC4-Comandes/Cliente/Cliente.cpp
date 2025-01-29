#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h> 

#pragma comment(lib, "ws2_32.lib")

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(9100);
    InetPtonA(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error: No s'ha pogut connectar al servei." << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Connexió establerta amb el servei." << std::endl;
    std::cout << "Introduïu la vostra comanda: ";
    std::string order;
    std::getline(std::cin, order);

    send(clientSocket, order.c_str(), order.size(), 0);

    char buffer[1024] = { 0 };
    recv(clientSocket, buffer, sizeof(buffer), 0);

    std::cout << "Identificador rebut: " << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
