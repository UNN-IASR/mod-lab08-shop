#pragma once

#include <iostream>
#include <queue>
#include <random>
#include <thread>
#include <mutex>


double getP0(int poolCount, double r, int maxLength);

double getPN(int poolCount, double r, double P0, int maxLength);

double getR(double lambda, double nu);

int randomInInterval(int min, int max);

double factorial(double n);

struct Customer {
    int arrivalTime;
    int itemsCount;
};

struct Statistics {
    int customersServed;
    int customersUnserved;
    double probFail;
    double throughputRel;
    double throughputAbs;
};

class Shop {
private:
    int cashRegistersNumber;
    int customerIntensity;
    double processingSpeed;
    int queueLengthMax;
    std::queue<Customer> customerQueue;
    std::vector<int> checkoutTimes;
    std::mutex mtx;
    bool isFinished;
    int customersServed;
    int customersUnserved;
    double queueLengthTotal;
    double customerStayTimeTotal;
    double checkoutTimeTotal;
    double idleTimeTotal;
    double timeTotal;

public:
    Shop(int registers, int intensity, int speed, int maxQueue);

    void simulate(int simulationTime, double itemsAvg);

    Statistics printActualStatistics();

    Statistics printTheoreticStatistics(double intensity, double speed, int cashRegistersNumber,
        int queueLengthMax, double itemsAvg);

    void cashierProcess();

    void customerProcess(double itemsAvg);

    int getCurrentTime();

};