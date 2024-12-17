#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>  // Para usar rand y srand
#include <ctime>    // Para inicializar la semilla de aleatoriedad
#include <set>      // Para almacenar los nombres únicos

std::vector<SOCKET> clients;
std::vector<std::string> clientNames;  // Para almacenar los nombres de los clientes
std::mutex clientsMutex;

// Lista de nombres aleatorios
std::vector<std::string> names = { "Solar", "Coral", "Tito", "Bocadillo Gamba", "Jimmy", "Superposito" };
std::set<std::string> usedNames;  // Conjunto para almacenar los nombres ya asignados

void handle_client(SOCKET clientSocket, std::string clientName) {
    char buffer[1024];
    int bytesReceived;

    while ((bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytesReceived] = '\0';

        // Mostrar el mensaje recibido y el nombre del cliente
        std::cout << "Mensaje recibido de " << clientName << ": " << buffer << std::endl;

        // Retransmitir el mensaje a todos los demás clientes
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (size_t i = 0; i < clients.size(); i++) {
            if (clients[i] != clientSocket) {
                std::string fullMessage = clientName + ": " + buffer;
                send(clients[i], fullMessage.c_str(), fullMessage.size(), 0);
            }
        }
    }

    // Eliminar el cliente desconectado
    std::lock_guard<std::mutex> lock(clientsMutex);
    clients.erase(std::remove(clients.begin(), clients.end(), clientSocket), clients.end());
    clientNames.erase(std::remove(clientNames.begin(), clientNames.end(), clientName), clientNames.end());
    usedNames.erase(clientName);  // Eliminar el nombre del conjunto de usados
    closesocket(clientSocket);
}

std::string get_unique_name() {
    srand(time(0));  // Inicializar la semilla para generar números aleatorios
    std::string clientName;
    int randomIndex;

    // Elegir un nombre aleatorio que no esté en uso
    do {
        randomIndex = rand() % names.size();  // Elegir un nombre aleatorio
        clientName = names[randomIndex];
    } while (usedNames.find(clientName) != usedNames.end());  // Verificar si el nombre ya ha sido asignado

    usedNames.insert(clientName);  // Agregar el nombre al conjunto de usados
    return clientName;
}

int main() {
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "Error al iniciar Winsock: " << result << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, SOMAXCONN);

    std::cout << "Servidor en espera de conexiones..." << std::endl;

    while (true) {
        SOCKET clientSocket = accept(serverSocket, nullptr, nullptr);

        // Obtener un nombre único para el cliente
        std::string clientName = get_unique_name();

        std::lock_guard<std::mutex> lock(clientsMutex);
        clients.push_back(clientSocket);
        clientNames.push_back(clientName);  // Almacenar el nombre del cliente

        std::cout << "Cliente conectado " << clientName << "." << std::endl;

        std::thread(handle_client, clientSocket, clientName).detach();
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
