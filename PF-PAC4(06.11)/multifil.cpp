#include <iostream>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <chrono>
#include <mutex>
#include <algorithm>

const int META = 40;  // Establece la istancia a la meta 

std::mutex mtx; // Evita que dos hilos escriban en la consola al mismo tiempo

// Función para mover el cursor en la consola
void irAPosicion(int x, int y) {
    COORD pos = { static_cast<SHORT>(x), static_cast<SHORT>(y) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Clase para representar a cada participante
class Competidor {
public:
    std::string nombre;
    std::string simbolo;
    int distancia;
    int velocidadBase;
    int fila;
    int puesto;

    // Constructor
    Competidor(std::string n, std::string s, int vb, int f)
        : nombre(n), simbolo(s), distancia(0), velocidadBase(vb), fila(f), puesto(0) {}

    // Método para mover la mariposa
    void avanzarMariposa() {
        while (distancia < META) {
            int avance = velocidadBase + (rand() % 6 - 3);  // Variación aleatoria
            distancia += (avance > 0 ? avance : 0); // Evita retrocesos
            mtx.lock();  
            irAPosicion(0, fila); // Para colocarlo en la fila
            for (int i = 0; i < distancia; i++) {
                std::cout << simbolo; // Mostrar el símbolo del competidor (M)
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500)); // Pausa aleatoria posandose en una flor
        }
    }

    // Método para mover la capibara
    void avanzarCapibara() {
        while (distancia < META) {
            // Probabilidad de detenerse "para comer"
            if (rand() % 10 == 0) {
                std::this_thread::sleep_for(std::chrono::seconds(2)); // Se detiene por dos segundos
            }
            // Probabilidad aumentada de avanzar más rápido
            if (rand() % 10 == 0) {
                velocidadBase += 4;  // Aumenta la velocidad
            }

            // Avance
            int avance = velocidadBase + (rand() % 6 - 3);
            distancia += (avance > 0 ? avance : 0);

            mtx.lock();
            irAPosicion(0, fila); // Posicionarse en la fila
            for (int i = 0; i < distancia; i++) {
                std::cout << simbolo; // Mostrar el símbolo
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500));
        }
    }

    // Método para mover la orca
    void avanzarOrca() {
        while (distancia < META) {
            // Probabilidad de dormirse
            if (rand() % 15 == 0) {
                std::this_thread::sleep_for(std::chrono::seconds(3)); // Duerme por 3 segundos
            }
            // Avance 
            int avance = velocidadBase + (rand() % 6 - 3);
            distancia += (avance > 0 ? avance : 0);

            mtx.lock();
            irAPosicion(0, fila);
            for (int i = 0; i < distancia; i++) {
                std::cout << simbolo;
            }
            mtx.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500));
        }
    }
};

// Función para mover cada competidor en un hilo
void moverCompetidor(Competidor& comp, int& puestosAsignados) {
    if (comp.nombre == "Mariposa") {
        comp.avanzarMariposa();
    }
    else if (comp.nombre == "Capibara") {
        comp.avanzarCapibara();
    }
    else if (comp.nombre == "Orca") {
        comp.avanzarOrca();
    }

    // Asigna puesto cuando llega a la meta
    mtx.lock();
    comp.puesto = ++puestosAsignados; 
    mtx.unlock();
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Semilla para números aleatorios, permite que la secuencia de números sea diferente en cada ejecución

    Competidor competidores[] = {
        {"Mariposa", "M", 5 + rand() % 2, 3}, // Lo primero es el valor que  se asigna al nombre del Competidor, lo segundo es el valor del símbolo, lo siguiente la velocidad base y lo último asigna la fila donde se verá el competidor en la consola.
        {"Capibara", "C", 5 + rand() % 3, 4}, //
        {"Orca", "O", 6 + rand() % 3, 5}
    };
    // \U0001F98B \U0001F42F  

    int puestosAsignados = 0;  // Contador de puestos
    // cuando uno de los competidores alcanza la meta 
    // el código utiliza puestosAsignados para asignar el puesto final al competidor 
    // cada vez que un competidor llega se incrementa 1 y se  asigna el siguiente puesto (puesto 1, 2, 3)

    std::cout << "¡La carrera ha comenzado!\n";
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    // Creación hilos para cada competidor
    std::thread hilos[3];
    for (int i = 0; i < 3; i++) {
        hilos[i] = std::thread(moverCompetidor, std::ref(competidores[i]), std::ref(puestosAsignados));
    }

    // Espera a que los hilos terminen
    for (int i = 0; i < 3; i++) {
        if (hilos[i].joinable()) hilos[i].join();
    }

    // Muestra resultados
    std::cout << "\n\n\n\nResultados:\n";

    // ordena por puesto el array entero
    std::sort(std::begin(competidores), std::end(competidores), [](const Competidor& a, const Competidor& b) { // función lambda que define el criterio de ordenación, dos objetos Competidor y devuelve true si el puesto del primer competidor es menor que el del segundo
        return a.puesto < b.puesto;
     });

    // Imprime resultados
    for (const auto& comp : competidores) {
        std::cout << "Puesto " << comp.puesto << ": " << " (" << comp.simbolo << ") " << comp.nombre << "\n";
    }

    return 0;
}
