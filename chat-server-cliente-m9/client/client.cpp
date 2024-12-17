#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <string>
#include <thread>

void receive_messages(SOCKET clientSocket) {
    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';  // Asegúrate de que el mensaje esté bien terminado con '\0'
        std::cout << buffer << std::endl;  // Mostrar el mensaje recibido, sin el prefijo "Mensaje del servidor:"
    }
}

int main() {
    // Inicializar Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);  // Inicializa Winsock
    if (result != 0) {
        std::cerr << "Error al iniciar Winsock: " << result << std::endl;
        return 1;
    }

    // Crear socket
    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);

    // Usar inet_pton para convertir la dirección IP
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    // Conectar al servidor
    result = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if (result == SOCKET_ERROR) {
        std::cerr << "No se pudo conectar al servidor: " << WSAGetLastError() << std::endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Conectado al servidor." << std::endl;

    // Hilo para recibir mensajes del servidor
    std::thread(receive_messages, clientSocket).detach();

    // Enviar mensajes al servidor
    std::string message;
    while (true) {
        std::cout << "Yo: ";
        std::getline(std::cin, message);
        if (message == "exit") {
            break; // Si escriben "exit", el cliente se desconecta
        }
        send(clientSocket, message.c_str(), message.size(), 0);
    }

    // Cerrar el socket y limpiar Winsock
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
