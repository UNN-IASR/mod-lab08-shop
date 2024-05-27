#include "task.h"

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    Supermarket supermarket(5, 10, 1, 10, 20);
    supermarket.runSimulation(60); // Run simulation for 60 seconds
    return 0;
}
