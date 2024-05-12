// Copyright 2024 Yuriy Nazarov
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <string>
#include "task.h"

int main(){
    setlocale(LC_ALL, "Rus");
    double numberRegisters = 5;
    double intensity = 20;
    double speed = 0.2;
    double avgProducts = 10;
    double maxQueue = 5;

    Shop shop(numberRegisters, intensity, speed, avgProducts, maxQueue);
    shop.StartWork();
    shop.CalculateStatistics();

    std::cout << "Обслуженные покупатели: " << shop.countServedClients << std::endl;
    std::cout << "Необслуженные покупатели: " << shop.countUnservedClients << std::endl << std::endl;

    std::cout << "Средняя длина очереди: " << shop.avgQueueLength << std::endl;
    std::cout << "Среднее время нахождение покупателя в очереди: " << shop.avgTimeInQueue << std::endl;
    std::cout << "Среднее время нахождение покупателя на кассе: " << (int)shop.avgServiceTime << std::endl << std::endl;
    std::cout << "Среднее время работы кассы: " << shop.avgWorkCashTime << std::endl;
    std::cout << "Среднее время простоя кассы: " << shop.avgWaitCashTime << "\n" << std::endl;

    std::cout << "Практические данные" << std::endl;
    std::cout << "Вероятность отказа: " << shop.pPn << std::endl;
    std::cout << "Относительную пропускную способность магазина: " << shop.pQ << std::endl;
    std::cout << "Абсолютную пропускную способность: " << shop.pA << std::endl << std::endl;

    std::cout << "Теоретические расчеты" << std::endl;
    std::cout << "Вероятность отказа: " << shop.tPn << std::endl;
    std::cout << "Относительная пропускная способность магазина: " << shop.tQ << std::endl;
    std::cout << "Абсолютная пропускная способность: " << shop.tA << std::endl << std::endl;
    return 0;
}