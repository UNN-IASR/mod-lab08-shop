#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <deque>
#include <vector>

class Customer {
 public:
	int countItems;
    double timeInQueue = 0;
    double timeInCass = 0;
    std::chrono::system_clock::time_point timeStart;
};

class Cassa {
 public:
    bool isWork = false;
    double timeWork = 0;
    double timePause = 0;
    std::chrono::system_clock::time_point timeStart;
};

class Shop {
 public:
    //входные параметры магазина
    int numberCass = 0; //количество касс
    double intensityOfCustomerFlow = 0.0; //интенсивность потока покупателей
    double speedProcessingCheckout = 0.0; //скорость обработки товара на кассе
    int averageOfItemInCart = 0; // среднее количество товаров в тележке покупателя
    int maxQueueLength = 0; //максимальная длина очереди

    //параментры, высчитываеме внутри
    int countCustomers = 0;
    std::queue<Customer> queueCustomers;//очередь клиентов
    std::vector<Cassa> vectorCass; //кассы - потоки
    std::vector<std::thread*> threadCass;
    std::mutex myMutex;

    //статистика
    std::chrono::system_clock::time_point allTimeStart;
    int allTime;

    int servedCustomers = 0; //обслуженные клиенты
    int unservedCustomers = 0;  //необслуженные клиенты
    double averageLenghtQueue = 0; //средняя длина очереди
    double averageTimeInQueue = 0; //среднее время нахождения в очереди
    double averageTimeInCass = 0; //среднее время нахождения на кассе
    double averageTimeWorkCass = 0; //Среднее время работы кассы
    double averageTimeSleepCass = 0; //Среднее время простоя кассы

 public:
    Shop(int n1, double n2, double n3, int n4, int n5); //запуск магазина с параметрами
    ~Shop();
    void Start();
    void Cash(int i, Customer customer, double speed, int j);
    void Stata();
};

int Factorial(int i);
