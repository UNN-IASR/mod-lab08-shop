// Copyright 2024 Alexandr Kokorin
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

class Shop {
public:
    Shop(int, double, double, double, int);
    ~Shop();
    int numOfCashRegisters;
    double buyerArrivalIntensity;
    double productProcessingSpeed;
    double AverageNumberOfGoods;
    int maxQueueLength;
    void StartWork();
    void ServeTheClient(int index, Client client);
    int allTime;
    std::chrono::system_clock::time_point allTimeStart;
    int countServedClients;
    int countUnservedClients;
    double queueLength;
    double timeInQueue;
    double serviceTime;
    std::vector<Cash> cashes;
private:
    std::queue<Client> clientsQueue;
    std::mutex myMutex;
    std::vector<std::thread*> threads;
};

double factorial(double n);
