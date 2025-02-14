#include <iostream>
#include <winsock2.h>
#include <thread>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

enum class Jugada { Piedra = 1, Papel, Tijeras, Lagarto, Spock, Ninguna };

Jugada determinarGanador(Jugada j1, Jugada j2) {
    if (j1 == j2) {
        return Jugada::Ninguna; 
    }

    switch (j1) {
    case Jugada::Piedra:
        return (j2 == Jugada::Tijeras || j2 == Jugada::Lagarto) ? Jugada::Piedra : Jugada::Papel;
    case Jugada::Papel:
        return (j2 == Jugada::Piedra || j2 == Jugada::Spock) ? Jugada::Papel : Jugada::Tijeras;
    case Jugada::Tijeras:
        return (j2 == Jugada::Papel || j2 == Jugada::Lagarto) ? Jugada::Tijeras : Jugada::Piedra;
    case Jugada::Lagarto:
        return (j2 == Jugada::Spock || j2 == Jugada::Papel) ? Jugada::Lagarto : Jugada::Tijeras;
    case Jugada::Spock:
        return (j2 == Jugada::Tijeras || j2 == Jugada::Piedra) ? Jugada::Spock : Jugada::Lagarto;
    default:
        return Jugada::Ninguna;
    }
}

void manejarPartida(SOCKET serverSocket, sockaddr_in& clientAddr1, sockaddr_in& clientAddr2) {
    char buffer[1024];
    int result;
    int clientAddrSize1 = sizeof(clientAddr1);
    int clientAddrSize2 = sizeof(clientAddr2);

    result = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&clientAddr1, &clientAddrSize1);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error al recibir datos del primer cliente." << std::endl;
        return;
    }

    int jugadaRecibida1 = *reinterpret_cast<int*>(buffer);
    Jugada jugada1 = static_cast<Jugada>(jugadaRecibida1);

    std::cout << "Jugador 1 ha elegido: " << static_cast<int>(jugada1) << std::endl;

    result = recvfrom(serverSocket, buffer, sizeof(buffer), 0, (SOCKADDR*)&clientAddr2, &clientAddrSize2);
    if (result == SOCKET_ERROR) {
        std::cerr << "Error al recibir datos del segundo cliente." << std::endl;
        return;
    }

    int jugadaRecibida2 = *reinterpret_cast<int*>(buffer);
    Jugada jugada2 = static_cast<Jugada>(jugadaRecibida2);

    std::cout << "Jugador 2 ha elegido: " << static_cast<int>(jugada2) << std::endl;

    Jugada ganador = determinarGanador(jugada1, jugada2);
    const char* mensajeJugador1 = (ganador == Jugada::Ninguna) ? "Empate!" : (ganador == jugada1) ? "Ganaste!" : "Perdiste!";
    const char* mensajeJugador2 = (ganador == Jugada::Ninguna) ? "Empate!" : (ganador == jugada2) ? "Ganaste!" : "Perdiste!";

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
