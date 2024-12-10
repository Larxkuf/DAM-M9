#include <winsock2.h>   
#include <ws2tcpip.h>   
#include <iostream>
#pragma comment(lib, "ws2_32.lib") 

#define PORT 9090
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error en inicialitzar Winsock" << std::endl;
        return -1;
    }

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error en crear el socket" << std::endl;
        WSACleanup();
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        std::cerr << "IP invàlida o no suportada" << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
        std::cerr << "Error en connectar amb el servidor: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }

    std::cout << "Connexió establerta amb el servidor.\n";

    const char* message = "Hola servidor";
    if (send(sock, message, strlen(message), 0) == SOCKET_ERROR) {
        std::cerr << "Error en enviar el missatge: " << WSAGetLastError() << std::endl;
        closesocket(sock);
        WSACleanup();
        return -1;
    }
    std::cout << "Enviant missatge: " << message << "\n";

    int valread = recv(sock, buffer, BUFFER_SIZE, 0);
    if (valread > 0) {
        std::cout << "Resposta del servidor: " << buffer << "\n";
    }
    else if (valread == 0) {
        std::cerr << "Conexió tancada pel servidor.\n";
    }
    else {
        std::cerr << "Error al rebre les dades: " << WSAGetLastError() << "\n";
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
