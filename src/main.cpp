// Copyright 2024 Kapochkin Mikhail

#include "../include/task.h"
#include <iostream>

int fact(int n) {
    if (n <= 1) return 1;
    return n * fact(n - 1);
}

int main() {
    int numCashMachines = 4;
    int queueLength = 10;
    int clientArrivalRate = 4;
    int cashMachineDelay = 10;
    int avgNumProducts = 4;
    int numClients = 40;

    double lambda = clientArrivalRate;
    double mu = 1000.0 / cashMachineDelay / avgNumProducts;
    double rho = mu / lambda;

    double P0 = 1;
    for (int i = 1; i <= numCashMachines; i++) {
        P0 += pow(rho, i) / fact(i);
    }
    for (int i = 1; i <= queueLength; i++) {
        P0 += pow(rho, numCashMachines + i)
            / (pow(numCashMachines, i) * fact(i));
    }

    double P_rej = pow(rho, numCashMachines + queueLength) * P0
        / (pow(numCashMachines, queueLength) * fact(numCashMachines));
    double Q = 1 - P_rej;
    double A = lambda * Q;

    std::cout << "Reject probability:: " << P_rej << std::endl;
    std::cout << "Relative throughput: "
        << Q << std::endl;
    std::cout << "Absolute Throughput: "
        << A << std::endl << std::endl;

    SuperMarket superMarket = SuperMarket(numCashMachines, queueLength, cashMachineDelay);
    superMarket.SimulateWork(numClients, clientArrivalRate, avgNumProducts);

    std::cout << "Total clients: " << superMarket.statistic.rejClients + superMarket.statistic.appClients << std::endl;
    std::cout << "Accepted clients: " << superMarket.statistic.appClients << std::endl;
    std::cout << "Rejected clients: " << superMarket.statistic.rejClients << std::endl;
    std::cout << "Reject probability: " << (double)superMarket.statistic.rejClients / numClients << std::endl;
    std::cout << "Relative throughput: "
        << (double)superMarket.statistic.appClients / numClients << std::endl;
    std::cout << "Absolute Throughput: "
        << (double)superMarket.statistic.appClients / numClients * clientArrivalRate << std::endl;
}