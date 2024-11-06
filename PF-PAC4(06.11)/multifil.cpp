#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <chrono>

const int META = 60;  // Distancia que deben alcanzar los animales para ganar

// Función para mover el cursor en la consola
void irAPosicion(int x, int y) {
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Clase para representar a cada participante en la carrera
class Competidor {
public:
    std::string nombre;
    int distancia;
    int velocidadBase;
    int fila; // Línea de la consola donde se muestra el progreso del competidor

    Competidor(std::string n, int vb, int f) : nombre(n), distancia(0), velocidadBase(vb), fila(f) {}

    // Simula el movimiento del competidor hacia la meta
    void avanzar() {
        while (distancia < META) {
            int avance = velocidadBase + (rand() % 3 - 1);  // Variación aleatoria de la velocidad
            distancia += std::max(0, avance); // Evita que retroceda
            irAPosicion(distancia, fila);
            std::cout << nombre[0]; // Muestra la primera letra del nombre del competidor
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500)); // Pausa aleatoria
        }
    }
};

// Clase para gestionar la carrera y sus competidores
class Carrera {
public:
    std::vector<Competidor> competidores;
    std::vector<std::thread> hilos;
    bool carreraFinalizada = false;

    Carrera() {
        srand(static_cast<unsigned int>(time(0))); // Inicializa la semilla para números aleatorios
    }

    void agregarCompetidor(const std::string& nombre, int velocidadBase, int fila) {
        competidores.emplace_back(nombre, velocidadBase, fila);
    }

    void iniciarCarrera() {
        for (Competidor& comp : competidores) {
            hilos.emplace_back(&Carrera::moverCompetidor, this, std::ref(comp));
        }

        // Espera a que todos los hilos terminen
        for (std::thread& hilo : hilos) {
            if (hilo.joinable()) hilo.join();
        }
    }

    void moverCompetidor(Competidor& comp) {
        while (!carreraFinalizada && comp.distancia < META) {
            comp.avanzar();
            if (comp.distancia >= META) {
                carreraFinalizada = true;
                irAPosicion(0, competidores.size() + 2); // Anuncia el ganador en una línea separada
                std::cout << "¡El ganador es: " << comp.nombre << "!\n";
            }
        }
    }
};

int main() {
    Carrera carrera;

    // Define los competidores con su velocidad base y línea de visualización
    carrera.agregarCompetidor("Mosca", 2, 3);
    carrera.agregarCompetidor("Capibara", 1, 4);
    carrera.agregarCompetidor("Manatí", 1, 5);

    std::cout << "¡La carrera de animales comienza! ¿Quién llegará primero a la meta?\n";
    std::cout << "---------------------------------------------------------------\n";

    // Inicia la carrera
    carrera.iniciarCarrera();

    return 0;
}











#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <chrono>

// Distancia para alcanzar la meta
const int META = 60; 

// Mueve el cursor a la posición en la consola
void irAPosicion(int x, int y) {
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Clase Competidor para representar a cada animal en la carrera
class Competidor {
public:
    std::string nombre;
    std::string icono;
    int posicion;
    int velocidadBase;
    int fila;  // Línea en la consola donde el animal avanza

    Competidor(std::string n, std::string i, int vb, int f)
        : nombre(n), icono(i), posicion(0), velocidadBase(vb), fila(f) {}

    // Simula el avance del animal hacia la meta
    void avanzar() {
        while (posicion < META) {
            int avance = velocidadBase + (rand() % 3 - 1);  // Aleatoriedad en la velocidad
            posicion += std::max(0, avance);  // Evita valores negativos
            irAPosicion(posicion, fila);
            std::cout << icono;  // Muestra el icono del animal
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 500));  // Pausa
        }
    }
};

// Clase Carrera para gestionar la competencia
class Carrera {
public:
    std::vector<Competidor> competidores;
    std::vector<std::thread> hilos;
    bool carreraFinalizada = false;

    Carrera() {
        srand(static_cast<unsigned int>(time(0)));  // Inicializa la semilla aleatoria
    }

    // Agrega un competidor a la carrera
    void agregarCompetidor(const std::string& nombre, const std::string& icono, int velocidadBase, int fila) {
        competidores.emplace_back(nombre, icono, velocidadBase, fila);
    }

    // Inicia la carrera
    void iniciarCarrera() {
        for (Competidor& comp : competidores) {
            hilos.emplace_back(&Carrera::moverCompetidor, this, std::ref(comp));
        }

        // Espera que todos los hilos terminen
        for (std::thread& hilo : hilos) {
            if (hilo.joinable()) hilo.join();
        }
    }

    // Función que controla el movimiento de cada competidor
    void moverCompetidor(Competidor& comp) {
        while (!carreraFinalizada && comp.posicion < META) {
            comp.avanzar();
            if (comp.posicion >= META) {
                carreraFinalizada = true;
                irAPosicion(0, competidores.size() + 3); // Anuncia al ganador
                std::cout << "¡El ganador es: " << comp.nombre << "!\n";
            }
        }
    }
};

int main() {
    Carrera carrera;

    // Agrega a Mosca, Capibara y Manatí con sus velocidades y líneas específicas
    carrera.agregarCompetidor("Mosca", "𓆦", 3, 3);
    carrera.agregarCompetidor("Capibara", "⠿⠿⠤⠤⠤⠤", 1, 4);
    carrera.agregarCompetidor("Manatí", "░░▒▒██", 2, 5);

    std::cout << "¡La carrera de animales comienza! ¿Quién llegará primero a la meta?\n";
    std::cout << "---------------------------------------------------------------\n";

    // Inicia la carrera
    carrera.iniciarCarrera();

    return 0;
}






















#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <windows.h>

const int FINISH_LINE = 50;  // The position to reach to win the race

// Utility function to move the console cursor
void moveToPosition(int x, int y) {
    COORD pos = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// Animal class to represent each participant in the race
class Animal {
public:
    std::string name;
    int position;
    int baseSpeed;
    int line; // The console line where the animal's progress is displayed

    Animal(std::string n, int bs, int l) : name(n), position(0), baseSpeed(bs), line(l) {}

    // Simulate the animal's movement in the race
    void run() {
        while (position < FINISH_LINE) {
            int move = baseSpeed + (rand() % 3 - 1);  // Randomize speed slightly (-1, 0, +1)
            position += std::max(0, move); // Prevent negative movement
            moveToPosition(position, line);
            std::cout << name[0]; // Display first letter of the animal's name as marker
            std::this_thread::sleep_for(std::chrono::milliseconds(100 + rand() % 400)); // Random pause
        }
    }
};

// Race manager to handle the animals and race flow
class Race {
public:
    std::vector<Animal> animals;
    std::vector<std::thread> threads;
    bool raceOver = false;

    Race() {
        // Seed random number generator
        srand(static_cast<unsigned int>(time(0)));
    }

    void addAnimal(const std::string& name, int baseSpeed, int line) {
        animals.emplace_back(name, baseSpeed, line);
    }

    void startRace() {
        for (Animal& animal : animals) {
            threads.emplace_back(&Race::raceAnimal, this, std::ref(animal));
        }

        // Join threads to wait for all animals to finish
        for (std::thread& t : threads) {
            if (t.joinable()) t.join();
        }
    }

    void raceAnimal(Animal& animal) {
        while (!raceOver && animal.position < FINISH_LINE) {
            animal.run();
            if (animal.position >= FINISH_LINE) {
                raceOver = true;
                moveToPosition(0, animals.size() + 2); // Announce winner below race lines
                std::cout << "Winner: " << animal.name << " reached the finish line!\n";
            }
        }
    }
};

int main() {
    Race race;

    // Define animals with base speed and console line positions
    race.addAnimal("Tortoise", 1, 3);
    race.addAnimal("Hare", 2, 4);
    race.addAnimal("Dog", 3, 5);

    std::cout << "Animal Race Begins! Reach the finish line to win!\n";
    std::cout << "------------------------------------------------\n";

    // Start the race
    race.startRace();

    return 0;
}
