#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <mutex>
#include <thread>

class Client {
public:
    std::vector<int> receipt;
    Client(std::vector<int> receipt);
};

class Shop {
private:
    // входные данные
    int cashboxAmount; // количество касс
    int clientsIntensity; // интенсивность потока покупателей
    int processingSpeed; // скорость обработки товара на кассе
    int averageQuantity; // среднее количество товаров в тележке покупателя
    int maximumQueue; // максимальная длина очереди


    // внутренние данные
    int queueLength = 3;
    bool isFinished = false;
    int numberOfWorkingCashboxes = 0;
    int numberOfQueues = 0;
    int numberOfReceipts = 0;
    std::vector<std::thread*> workingCashboxes;
    std::vector<std::queue<Client*>*> queues;
    std::mutex mutex;

    // сбор статистики
    int servedClients = 0; // обслуженные клиенты
    int notServedClients = 0; // необслуженные клиенты
    double averageQueueLength = 0.0; // средняя длина очереди
    double averageClientTime = 0.0; // среднее время нахождение покупателя в очереди и на кассе
    double averageCashboxWorktime = 0.0; // среднее время работы кассы
    double averageCashboxDowntime = 0.0; // среднее время простоя кассы

public:
    Client* getClient();
    Shop(int cashboxAmount, int clientsIntensity, int processingSpeed, int averageQuantity, int maximumQueue);

    void startShopping();
    void serveClient(Client* client, int number);
    void serveQueue(std::queue<Client*>* customers);
    void serveShop();

    int getAmountOfServedClients();
    int getAmountOfNotServedClients();
    double getAverageQueueLength();
    double getAverageClientTime();
    double getAverageCashboxWorktime();
    double getAverageCashboxDowntime();

    // дополнительные расчеты
    double factorial(double n);
    double getPrej(); // вероятность отказа
    double getQ(); // относительная пропускная способность
    double getA(); // абсолютная пропускная способность
};
