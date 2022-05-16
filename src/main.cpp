// Copyright 2022 UNN-IASR
#include "task.h"
#include <iostream>

int main()
{
    int applicationFlow = 100;
    int serviceFlow = 20;
    int maxBayers = 100;
    int queueLen = 3;
    int averageNumberOfItems = 5;
    int maxPrice = 40;
    int goodsService = (int)(applicationFlow / averageNumberOfItems);
    int buyerIntensity = serviceFlow;
    int numberOfCashDesks = 2;
    Market* market = new Market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market->run();
    std::cout << "Отказов: "                       << market->getStats().failed_count << std::endl;
    std::cout << "Успехов: "                       << market->getStats().processed_count << std::endl;
    std::cout << "Запросов: "                      << market->getStats().request_count << std::endl;
    std::cout << "Средняя длина очереди: "         << market->getAverageQueueLength() << std::endl;
    std::cout << "Среднее время ожидания: "        << market->getAverageWaitTime() << std::endl;
    std::cout << "Среднее время на обслуживание: " << market->getAverageCheckoutTime() << std::endl;
    std::cout << "Среднее время простоя: "         << market->getAverageIdleTime() << std::endl;
}
