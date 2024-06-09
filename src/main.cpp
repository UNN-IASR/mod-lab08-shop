#include "../include/task.h"

int main() {
    int cashRegisters = 2;
    int customerIntensity = 10;
    int speed = 10;
    int queueLengthMax = 10;
    int simulationTime = 20;
    double itemsAvg = 3;

    Shop shop(cashRegisters, customerIntensity, speed, queueLengthMax);
    shop.simulate(simulationTime, itemsAvg);

    shop.printActualStatistics();
    shop.printTheoreticStatistics(customerIntensity, speed, cashRegisters, queueLengthMax, itemsAvg);
    return 0;
}