// Copyright 2024 Alexandr Kokorin
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <string>
#include "task.h"

int main() {
    setlocale(LC_ALL, "Rus");
    double numberRegisters = 5;
    double intensity = 20;
    double speed = 0.2;
    double avgProducts = 10;
    double maxQueue = 5;

    Shop shop(numberRegisters, intensity, speed, avgProducts, maxQueue);
    shop.StartWork();

    double avgWorkCashTime = 0;
    double avgWaitCashTime = 0;

    for (int i = 0; i < shop.numOfCashRegisters; i++) {
        avgWorkCashTime += shop.cashes[i].workTime;
        avgWaitCashTime += shop.cashes[i].waitTime;
    }

    double avgQueueLength = shop.queueLength / shop.buyerArrivalIntensity;
    double avgTimeInQueue = shop.timeInQueue / shop.buyerArrivalIntensity;
    double avgServiceTime = shop.serviceTime / shop.buyerArrivalIntensity;

    avgWorkCashTime /= shop.numOfCashRegisters;
    avgWaitCashTime /= shop.numOfCashRegisters;

    double la = shop.buyerArrivalIntensity / shop.allTime;
    double mu = (double)shop.countServedClients / shop.allTime;
    double p = la / mu;

    double pPn = shop.countUnservedClients / shop.buyerArrivalIntensity;
    double pQ = 1.0 - pPn;
    double pA = la * pQ;

    double P0 = 1.0;
    for (int i = 1; i <= shop.numOfCashRegisters; i++) {
        P0 = P0 + pow(p, i) / factorial(i);
    }
    for (int i = shop.numOfCashRegisters + 1; i < shop.numOfCashRegisters + shop.maxQueueLength; i++) {
        P0 = P0 + pow(p, i) / (factorial(shop.numOfCashRegisters) * pow(shop.numOfCashRegisters, i - shop.numOfCashRegisters));
    }
    P0 = 1.0 / P0;
    double tPn = P0 * pow(p, (shop.numOfCashRegisters + shop.maxQueueLength)) / (double)(pow(shop.numOfCashRegisters, shop.maxQueueLength) * factorial(shop.numOfCashRegisters));
    double tQ = 1.0 - tPn;
    double tA = la * tQ;

    std::cout << "Обслуженные покупатели: " << shop.countServedClients << std::endl;
    std::cout << "Необслуженные покупатели: " << shop.countUnservedClients << std::endl << std::endl;

    std::cout << "Средняя длина очереди: " << avgQueueLength << std::endl;
    std::cout << "Среднее время нахождение покупателя в очереди: " << avgTimeInQueue << std::endl;
    std::cout << "Среднее время нахождение покупателя на кассе: " << (int)avgServiceTime << std::endl << std::endl;
    std::cout << "Среднее время работы кассы: " << avgWorkCashTime << std::endl;
    std::cout << "Среднее время простоя кассы: " << avgWaitCashTime << "\n" << std::endl;

    std::cout << "Практические данные" << std::endl;
    std::cout << "Вероятность отказа: " << pPn << std::endl;
    std::cout << "Относительную пропускную способность магазина: " << pQ << std::endl;
    std::cout << "Абсолютную пропускную способность: " << pA << std::endl << std::endl;

    std::cout << "Теоретические расчеты" << std::endl;
    std::cout << "Вероятность отказа: " << tPn << std::endl;
    std::cout << "Относительная пропускная способность магазина: " << tQ << std::endl;
    std::cout << "Абсолютная пропускная способность: " << tA << std::endl << std::endl;
    return 0;
}
