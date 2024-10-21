#include <iostream>
#include <unistd.h>   // fork(), wait()
#include <sys/wait.h>
#include <cstdlib>    
#include <ctime>      

using namespace std;

// Definición variable global (array)
string losnombres[10] = {"Luis", "Andrea", "Dalia", "Agrippina", "Samhainophobia", "Michaela", "Ezra", "Kai", "Rai", "Kia"}; // los nombres que hay son solo para rellenar

// Función que aleatoriamente escoge a una persona de la array y muestra una frase donde mencione la persona
void escogerNombre(int proceso) {
    srand(getpid() + time(NULL));  
    int indice = rand() % 10;    // nombre al azar
    cout << "Proceso " << proceso << ": El nombre escogido ha sido " << losnombres[indice] << endl;
}

int main() {
    int n;
    cout << "Introduce la cantidad de procesos a crear: ";
    cin >> n; // N procesos (la cantidad la escoge el usuario)

    for (int i = 0; i < n; ++i) {
        pid_t pid = fork();  // Esto crea un proceso hijo
        if (pid < 0) { // Si falla
            cerr << "Error al intentar crear el proceso hijo" << endl;
            return 1;
        } else if (pid == 0) {
            // Proceso hijo
            cout << "Proceso hijo iniciado. PID: " << getpid() << endl;

            int posicion;
            cout << "Proceso hijo " << i + 1 << ": Introduce la posición (0-9) del nombre que deseas cambiar: ";
            cin >> posicion;
            if (posicion < 0 || posicion >= 10) {
                cerr << "Posición inválida. Debe estar 0-9." << endl;
                exit(1);  // Si la posición es inválida termina el proceso hijo
            }

            string nuevoNombre;
            cout << "Proceso hijo: Introduce un nuevo nombre para la posición " << posicion << "  para añadir al array: ";
            cin >> nuevoNombre;
            losnombres[posicion] = nuevoNombre; 

            escogerNombre(i + 1);  // Escoge y muestra un nombre aleatorio
            cout << "Proceso hijo finalizado." << endl;
            exit(0);  // El proceso h termina
        }
    } 

    // Espera a que todos los procesos hijos terminen
    for (int i = 0; i < n; ++i) {
        wait(NULL);
    }
    cout << "El proceso padre ha esperado a que terminen todos los procesos hijos." << endl;

    return 0;
}
