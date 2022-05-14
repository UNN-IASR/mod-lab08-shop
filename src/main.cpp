#include "task.h"
#include <iostream>

int main()
{
    int cashboxes = 2;
    int intensity = 30;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;

    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();

    std::cout << "Not served customers: " << supermarket.getAmountOfServedClients() << std::endl;
    std::cout << "Served customers: " << supermarket.getAmountOfNotServedClients() << std::endl;
    std::cout << "Average queue length: " << supermarket.getAverageQueueLength() << std::endl;
    std::cout << "Average client time: " << supermarket.getAverageClientTime() << std::endl;
    std::cout << "Average cashbox worktime: " << supermarket.getAverageCashboxWorktime() << std::endl;
    std::cout << "Average cashbox downtime: " << supermarket.getAverageCashboxDowntime() << std::endl;
    std::cout << "Prej: " << supermarket.getPrej() << std::endl;
    std::cout << "Q: " << supermarket.getQ() << std::endl;
    std::cout << "A: " << supermarket.getA() << std::endl;
}
