// Copyright 2024 Danil Volkov

#ifndef INCLUDE_TASK_H
#define INCLUDE_TASK_H

#include <mutex>
#include <vector>
#include <condition_variable>
#include <queue>
#include <chrono>
#include <fstream>
#include <random>
#include <thread>

struct ProcEventArgs {
    int id;
    std::vector<int> cart;
    std::chrono::steady_clock::time_point enterQueueTime;
    std::chrono::steady_clock::time_point enterCashierTime;
};

class Shop {
private:
    std::mutex threadLock;
    std::vector<std::thread> pool;
    std::queue<ProcEventArgs> requests;
    int requestCount = 0;
    int processedCount = 0;
    int rejectedCount = 0;
    double exitCashierTime = 0;
    std::condition_variable cv;
    bool stop = false;
    int maxQueueLength;
    std::vector<ProcEventArgs> processedCustomers;
    int totalQueueLength = 0;
    bool allCustomersProcessed = false;

public:
    double speedProductCash = 1.00;

    Shop(int,  int,  double);
    ~Shop();

    void proc(ProcEventArgs args);
    void answer();

    double getAverageQueueTime() const;
    double getAverageCashierTime() const;
    double getAverageQueueLength() const;

    void setAllCustomersProcessed();
    bool isAllCustomersProcessed() const;

    int getRequestCount();
    int getProcessedCount();
    int getRejectedCount();
};

class Client {
private:
    Shop* shop;
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution;

public:
    Client(Shop*, int);

    void send(int);
    void onProc(ProcEventArgs&);
};

unsigned long long factorial(int);

#endif // INCLUDE_TASK_H
