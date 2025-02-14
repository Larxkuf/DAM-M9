#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

enum class Jugada { Piedra = 1, Papel, Tijeras, Ninguna };

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al iniciar Winsock." << std::endl;
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (serverAddr.sin_addr.s_addr == INADDR_NONE) {
        std::cerr << "Dirección IP inválida." << std::endl;
        return 1;
    }

    int jugada;
    std::cout << "Elige que vas a sacar: 1. Piedra, 2. Papel, 3. Tijeras: ";
    std::cin >> jugada;

    sendto(clientSocket, (char*)&jugada, sizeof(jugada), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    char buffer[1024];
    int result = recvfrom(clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (result == SOCKET_ERROR) {
        std::cerr << "Ha habido un error recibiendo datos del servidor." << std::endl;
        return 1;
    }

    // Asegúrate de que el buffer esté correctamente terminado
    buffer[result] = '\0';  // Termina la cadena correctamente

    std::cout << "Resultado: " << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
