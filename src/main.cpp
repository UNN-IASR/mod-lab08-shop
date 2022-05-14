// Copyright 2022 UNN-IASR
#include "task.h"
#include <iostream>

int main()
{
    int applicationFlow = 100;
    int serviceFlow = 20;
    int maxPrice = 40;
    int maxBayers = 100;
    int queueLen = 3;
    int averageNumberOfItems = 5;
    int goodsService = (int)(applicationFlow / averageNumberOfItems);
    int buyerIntensity = serviceFlow;
    int numberOfCashDesks = 2;
    Market* market = new Market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market->run();
    std::cout << "Number of failures " << market->getStatistics().numberOfFailed << std::endl;
    std::cout << "Number of processed " << market->getStatistics().numberOfProcessed << std::endl;
    std::cout << "Number of requests " << market->getStatistics().numberOfRequests << std::endl;
    std::cout << "average queue length " << market->getAverageQueueLength() << std::endl;
    std::cout << "average time of waiting " << market->getAverageTimeOfWaiting() << std::endl;
    std::cout << "average checkout time " << market->getAverageCheckoutTime() << std::endl;
    std::cout << "average rest time " << market->getAverageRestTime() << std::endl;
}
