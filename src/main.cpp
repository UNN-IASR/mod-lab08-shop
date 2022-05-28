#include "task.h"
#include <iostream>

int main()
{
    int cashboxes = 3;
    int intensity = 40;
    int speed = 15;
    int averageProducts = 3;
    int clients = 3;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();

    std::cout << "Число необслуженных клиентов: " << supermarket.getAmountOfNotServedClients() << std::endl;
    std::cout << "Число обслуженных клиентов: " << supermarket.getAmountOfServedClients() << std::endl;
    std::cout << "Средняя длина очереди: " << supermarket.getAverageQueueLength() << std::endl;
    std::cout << "Среднее время пребывания покупателя в очереди и на кассе: " << supermarket.getAverageClientTime() << std::endl;
    std::cout << "Среднее время работы кассы: " << supermarket.getAverageCashboxWorktime() << std::endl;
    std::cout << "Среднее время простоя кассы: " << supermarket.getAverageCashboxDowntime() << std::endl;
    std::cout << "Вероятность отказа: " << supermarket.getPrej() << std::endl;
    std::cout << "относительная пропускная способность Q: " << supermarket.getQ() << std::endl;
    std::cout << "абсолютная пропускная способность A: " << supermarket.getA() << std::endl;
}
