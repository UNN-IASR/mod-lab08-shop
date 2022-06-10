// Copyright 2022 UNN
#include <iostream>
#include "task.h"

int main()
{
    setlocale(LC_ALL, "Russian");
    int cassNum = 5;
    int maxNumClients = 120;
    int intensClients = 15;
    int servSpeed = 30;
    int averItemNum = 10;
    int maxLen = 5;
    Market m(cassNum, maxNumClients, intensClients, servSpeed, averItemNum, maxLen);
    m.start();

    std::cout << std::endl << std::endl << "Итого:" << std::endl << std::endl;
    std::cout << "Необслуженных клиентов: " << m.getAmountOfUnservedClients() << std::endl;
    std::cout << "Обслуженных клиентов: " << m.getAmountOfServedClients() << std::endl;
    std::cout << "Средняя длина очереди: " << m.getAverageLineLength() << std::endl;
    std::cout << "Среднее время ожидания: " << m.getAverageWaitingTimeForClient() << std::endl;
    std::cout << "Среднее время работы: " << m.getAverageWorkTime() << std::endl;
    std::cout << "Среднее время простоя: " << m.getAverageDownTime() << std::endl;
}
