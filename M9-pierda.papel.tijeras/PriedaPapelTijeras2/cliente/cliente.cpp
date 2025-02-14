#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

enum class Jugada { Piedra = 1, Papel, Tijeras, Lagarto, Spock, Ninguna };

void mostrarReglas() {
    std::cout << "Reglas del juego Piedra, Papel, Tijeras, Lagarto, Spock:" << std::endl;
    std::cout << "1. Piedra aplasta Tijeras." << std::endl;
    std::cout << "2. Tijeras cortan Papel." << std::endl;
    std::cout << "3. Papel cubre Piedra." << std::endl;
    std::cout << "4. Piedra aplasta Lagarto." << std::endl;
    std::cout << "5. Lagarto envenena Spock." << std::endl;
    std::cout << "6. Spock destruye Tijeras." << std::endl;
    std::cout << "7. Tijeras decapitan Lagarto." << std::endl;
    std::cout << "8. Lagarto come Papel." << std::endl;
    std::cout << "9. Papel refuta Spock." << std::endl;
    std::cout << "10. Spock rompe Piedra." << std::endl << std::endl;
}

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

    mostrarReglas();

    int jugada;
    std::cout << "Elige tu jugada: " << std::endl;
    std::cout << "1. Piedra" << std::endl;
    std::cout << "2. Papel" << std::endl;
    std::cout << "3. Tijeras" << std::endl;
    std::cout << "4. Lagarto" << std::endl;
    std::cout << "5. Spock" << std::endl;
    std::cout << "Ingresa el número de la jugada: ";
    std::cin >> jugada;

    sendto(clientSocket, (char*)&jugada, sizeof(jugada), 0, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

    char buffer[1024];
    int result = recvfrom(clientSocket, buffer, sizeof(buffer), 0, nullptr, nullptr);
    if (result == SOCKET_ERROR) {
        std::cerr << "Ha habido un error recibiendo datos del servidor." << std::endl;
        return 1;
    }

    buffer[result] = '\0';  

    std::cout << "Resultado: " << buffer << std::endl;

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
