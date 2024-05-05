// Copyright 2024 alenkaLo
#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <future>
#include <cmath>
#include <queue>
#include <deque>
#include <vector>
#include <chrono>

class Client {
public:
    int products;
    int timeWaited;
    int timeServed;
    std::chrono::system_clock::time_point timeQueueStart;
};

class Cash {
public:
    int index;
    double workTime;
    double waitTime;
    std::chrono::system_clock::time_point timeDownStart;
    bool free;
};
class Shop {
public:
    //Входные параметры 
    double countCash;//количество касс
    double li;//интенсивность потока покупателей
    double mu;//скорость обработки товара на кассе
    double countProduct;//среднее количество товаров в тележке
    double maxlenQueue;//максимальная длина очереди

    std::mutex myMutex;

    std::queue<Client> queue;
    std::vector<Cash> cash;
    std::vector<std::thread*> ts;

    Shop(double numberRegisters, double intensity,
        double speed, double avgProducts, double maxQueue);
    void WorkCash(int id, Client client, double speed);//имитация раоты кассы
    void Work();
    void Statistics();

    //Статистика
    std::chrono::system_clock::time_point allTimeStart;
    int allTime;

    int servedСustomers; //обслуженные покупатели
    int unservedCustomers; //необслуженные покупатели

    double lenQueue;//средняя длина очереди
    double timeWaitedClient;//среднее время нахождение покупателя в очереди
    double timeServedClient;//cреднее время на кассе

    double timeWorkedCash;//среднее время работы кассы
    double timeWaitedCash;//среднее время простоя кассы

    //практические данные
    double pPn;//вероятность отказа
    double pQ;//относительную пропускную способность магазина
    double pA;//абсолютную пропускную способность

    //теоретические данные
    double tPn;//вероятность отказа
    double tQ;//относительную пропускную способность магазина
    double tA;//абсолютную пропускную способность
};

