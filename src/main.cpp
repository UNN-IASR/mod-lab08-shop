// Copyright 2022 UNN-IASR
#include "task.h"
#include <iostream>

int main()
{
    int cashRegisterAmount = 2;
    int flowRate = 500;
    int processingSpeed = 1000;
    int averageNumberOfProducts = 8;
    int maxQueueLength = 6;
    int maxBuyers = 10000;
    Supermarket* supermarket = new Supermarket (cashRegisterAmount,flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
    supermarket->run();
}
