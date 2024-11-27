#include <iostream>
#include <thread>

// números imparells de l'1 al 10 (1, 3, 5, 7, 9)
void imprimirImpares() {
    for (int i = 1; i <= 10; i += 2) {
        std::cout << i << std::endl;
    }
}
// números parells de l'1 al 10 (2, 4, 6, 8, 10).
void imprimirPares() {
    for (int i = 2; i <= 10; i += 2) {
        std::cout << i << std::endl;
    }
}

int main() {
    // los dos hilos
    std::thread filImpares(imprimirImpares);
    std::thread filPares(imprimirPares);

    // Assegura't que el programa espera que tots dos fils acabin abans de finalitzar, utilitzant join().
    filImpares.join();
    filPares.join();

    return 0;
}


