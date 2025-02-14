#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

enum class Jugada { Piedra = 1, Papel, Tijeras, Ninguna };

Jugada determinarGanador(Jugada j1, Jugada j2) {
    if (j1 == j2) {
        return Jugada::Ninguna;
    }

    switch (j1) {
    case Jugada::Piedra:
        return (j2 == Jugada::Tijeras) ? Jugada::Piedra : Jugada::Papel;
    case Jugada::Papel:
        return (j2 == Jugada::Piedra) ? Jugada::Papel : Jugada::Tijeras;
    case Jugada::Tijeras:
        return (j2 == Jugada::Papel) ? Jugada::Tijeras : Jugada::Piedra;
    default:
        return Jugada::Ninguna;
    }
}

void manejarPartida(SOCKET serverSocket, sockaddr_in& clientAddr1, sockaddr_in& clientAddr2) {
    char buffer[1024];
    int result;
    int clientAddrSize1 = sizeof(clientAddr1);
    int clientAddrSize2 = sizeof(clientAddr2);

    // Inicializa el buffer para evitar basura
    memset(buffer, 0, sizeof(buffer));

    // Recibe la jugada del primer jugador
    result = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&clientAddr1, &clientAddrSize1);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error al recibir datos del primer cliente." << std::endl;
        return;
    }

    int jugadaRecibida1 = *reinterpret_cast<int*>(buffer);
    Jugada jugada1 = static_cast<Jugada>(jugadaRecibida1);

    std::cout << "Jugador 1 ha elegido: " << static_cast<int>(jugada1) << std::endl;

    // Recibe la jugada del segundo jugador
    memset(buffer, 0, sizeof(buffer));  // Limpiar el buffer
    result = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&clientAddr2, &clientAddrSize2);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error al recibir datos del segundo cliente." << std::endl;
        return;
    }

    int jugadaRecibida2 = *reinterpret_cast<int*>(buffer);
    Jugada jugada2 = static_cast<Jugada>(jugadaRecibida2);

    std::cout << "Jugador 2 ha elegido: " << static_cast<int>(jugada2) << std::endl;

    // Determinar el ganador
    Jugada ganador = determinarGanador(jugada1, jugada2);
    const char* mensajeJugador1 = (ganador == Jugada::Ninguna) ? "Empate!" : (ganador == jugada1) ? "Ganaste!" : "Perdiste!";
    const char* mensajeJugador2 = (ganador == Jugada::Ninguna) ? "Empate!" : (ganador == jugada2) ? "Ganaste!" : "Perdiste!";

    // Enviar el resultado a los jugadores
    sendto(serverSocket, mensajeJugador1, strlen(mensajeJugador1), 0, (SOCKADDR*)&clientAddr1, clientAddrSize1);
    sendto(serverSocket, mensajeJugador2, strlen(mensajeJugador2), 0, (SOCKADDR*)&clientAddr2, clientAddrSize2);
}

int main() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error al iniciar Winsock." << std::endl;
        return 1;
    }

    SOCKET serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error al crear el socket." << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error al enlazar el socket." << std::endl;
        return 1;
    }

    std::cout << "Servidor escuchando en el puerto 8080..." << std::endl;

    sockaddr_in clientAddr1, clientAddr2;
    memset(&clientAddr1, 0, sizeof(clientAddr1));
    memset(&clientAddr2, 0, sizeof(clientAddr2));

    while (true) {
        std::cout << "Esperando dos jugadores..." << std::endl;

        manejarPartida(serverSocket, clientAddr1, clientAddr2);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
