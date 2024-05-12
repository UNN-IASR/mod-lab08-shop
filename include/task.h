// Copyright 2024 Yuriy Nazarov
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
    int numOfProducts;
    int waitTime;
    int serviceTime;
    std::chrono::system_clock::time_point timeQueueStart;
    Client(int);
    void SetWaitTime();
};

class Cash {
 public:
    int index;
    double productProcessingSpeed;
    double workTime;
    double waitTime;
    std::chrono::system_clock::time_point startTime;
    bool isFree;
    Cash(int, double);
    Cash();
};

class Shop{
 public: 
    Shop(int, double, double, double, int);
    ~Shop();
    int numOfCashRegisters;
    double buyerArrivalIntensity; //в мс
    double productProcessingSpeed; //в мс
    double AverageNumberOfGoods;
    int maxQueueLength;
    void StartWork();
    void ServeTheClient(int index, Client client);
    //Статистика
    void CalculateStatistics();
    int allTime;
    std::chrono::system_clock::time_point allTimeStart;
    int countServedClients;
    int countUnservedClients;
    double avgQueueLength;
    double avgTimeInQueue;
    double avgServiceTime;
    double avgWorkCashTime;
    double avgWaitCashTime;
    //практические данные
    double pPn; //Вероятность отказа
    double pQ;  //Относительная пропускная способность магазина
    double pA;  //Абсолютная пропускная способность
    //теоретические данные
    double tPn; //Вероятность отказа
    double tQ;  //Относительная пропускная способность магазина
    double tA;  //Абсолютная пропускная способность
 private: 
    std::queue<Client> clientsQueue;
    std::mutex myMutex;
    std::vector<Cash> cashes;
    std::vector<std::thread*> threads;
};

double factorial(double n);
