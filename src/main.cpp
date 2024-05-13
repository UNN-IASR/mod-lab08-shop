// Copyright 2024 Artyom Ignatyev

#include "../include/task.h"
#include <iostream>

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n-1);
}

int main() {
    int numCashRegisters = 3;
    int queueSize = 6;
    int clientArrivalRate = 25;
    int cashRegisterDelay = 45;
    int avgNumProducts = 4;
    int numClients = 100;

    double lambda = clientArrivalRate;
    double mu = 1000.0 / cashRegisterDelay / avgNumProducts;
    double rho = mu / lambda;

    double P0 = 1;
    for (int i = 1; i <= numCashRegisters; i++) {
        P0 += pow(rho, i) / factorial(i);
    }
    for(int i = 1; i <= queueSize; i++) {
        P0 += pow(rho, numCashRegisters + i) 
            / (pow(numCashRegisters, i) * factorial(i));
    }

    double P_rej = pow(rho, numCashRegisters + queueSize) * P0
                    / (pow(numCashRegisters, queueSize) * factorial(numCashRegisters));
    double Q = 1 - P_rej;
    double A = lambda * Q;

    std::cout << "Reject probability:: " << P_rej << std::endl;
    std::cout << "Relative throughput: " 
        << Q << std::endl;
    std::cout << "Absolute Throughput: " 
        << A << std::endl << std::endl;

    Shop shop = Shop(numCashRegisters, queueSize, cashRegisterDelay);
    shop.Simulate(numClients, clientArrivalRate, avgNumProducts);

    std::cout << "Total clients: " << shop.statistics.rejectedClients + shop.statistics.servedClients << std::endl;
    std::cout << "Accepted clients: " << shop.statistics.servedClients << std::endl;
    std::cout << "Rejected clients: " << shop.statistics.rejectedClients << std::endl;
    std::cout << "Reject probability: " << (double)shop.statistics.rejectedClients / numClients << std::endl;
    std::cout << "Relative throughput: " 
         << (double)shop.statistics.servedClients / numClients << std::endl;
    std::cout << "Absolute Throughput: " 
        << (double)shop.statistics.servedClients / numClients * clientArrivalRate << std::endl;
}