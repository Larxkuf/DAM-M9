#include <winsock2.h>  
#include <ws2tcpip.h>   
#include <iostream>
#pragma comment(lib, "ws2_32.lib") 

#define PORT 9090
#define BUFFER_SIZE 1024

int main() {
    WSADATA wsaData;
    SOCKET server_fd, client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = { 0 };

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error en inicialitzar Winsock" << std::endl;
        return -1;
    }

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Error en crear el socket" << std::endl;
        WSACleanup();
        return -1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; 
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) == SOCKET_ERROR) {
        std::cerr << "Error al bind: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    if (listen(server_fd, 3) == SOCKET_ERROR) {
        std::cerr << "Error al listen: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    std::cout << "Esperant connexions... \n";

    if ((client_fd = accept(server_fd, (struct sockaddr*)&address, &addrlen)) == INVALID_SOCKET) {
        std::cerr << "Error en el accept: " << WSAGetLastError() << std::endl;
        closesocket(server_fd);
        WSACleanup();
        return -1;
    }

    int valread = recv(client_fd, buffer, BUFFER_SIZE, 0);
    if (valread > 0) {
        std::cout << "Missatge rebut del client: " << buffer << "\n";

        const char* response = "Hola desdel servidor!";
        send(client_fd, response, strlen(response), 0);
        std::cout << "Resposta enviada al client.\n";
    }

    closesocket(client_fd);
    closesocket(server_fd);
    WSACleanup();

    return 0;
}
