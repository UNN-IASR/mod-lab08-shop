// Copyright 2024 alenkaLo
#include "task.h"
#include <iostream>


int main()
{
    setlocale(LC_ALL, "Rus");
    double numberRegisters = 5;
    double intensity = 20;
    double speed = 0.2;
    double avgProducts = 10;
    double maxQueue = 5;

    Shop shop(numberRegisters, intensity, speed, avgProducts, maxQueue);
    shop.Work();
    shop.Statistics();

    std::cout << "обслуженные покупатели: " << shop.servedСustomers << std::endl;
    std::cout << "еобслуженные покупатели: " << shop.unservedCustomers << std::endl << std::endl;

    std::cout << "средняя длина очереди: " << shop.lenQueue << std::endl;
    std::cout << "среднее время нахождение покупателя в очереди: " << shop.timeWaitedClient << std::endl;
    std::cout << "среднее время нахождение покупателя на кассе: " << (int)shop.timeServedClient << std::endl << std::endl;

    std::cout << "среднее время работы кассы: " << shop.timeWorkedCash << std::endl;
    std::cout << "среднее время простоя кассы: " << shop.timeWaitedCash << std::endl << std::endl;

    std::cout << "практические данные" << std::endl;
    std::cout << "вероятность отказа: " << shop.pPn << std::endl;
    std::cout << "относительную пропускную способность магазина: " << shop.pQ << std::endl;
    std::cout << "абсолютную пропускную способность: " << shop.pA << std::endl << std::endl;

    std::cout << "теоретические данные" << std::endl;
    std::cout << "вероятность отказа: " << shop.tPn << std::endl;
    std::cout << "относительную пропускную способность магазина: " << shop.tQ << std::endl;
    std::cout << "абсолютную пропускную способность: " << shop.tA << std::endl << std::endl;
    return 0;
}

