// Copyright 2024
#pragma once

#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

int GenRandom(int a, int b);
static int Factorial(int n);
class Shop {
private:
    int numCashReg;
    int flowIntens;
    int speedProc;
    int averageProd;
    int maxLength;
    int maxCustom = 50;
    int served = 0;
    double averageLen = 0;
    double averageTime = 0;
    double averageWorkCash = 0;
    double averageDownCash = 0;
    double time = 0;
    struct Customer {
        std::vector<int> shopList;
        Customer(const std::vector<int>& check) : shopList(check) {}
    };
    using queueCust = std::shared_ptr<std::queue<std::shared_ptr<Customer>>>;
    std::shared_ptr<Customer> GenCust();
    std::vector<std::thread> threads;
    std::vector<queueCust> queueB;
    std::mutex mutex;
public:
    Shop(int numCashReg, int flowIntens, int speedProc,
        int averageProd, int maxLength);
    void Run();
    void ServeCust(Customer& buyer);
    void ServeQ(queueCust queue);
    void Statistic();
    int NumCashReg();
    int FlowIntens();
    int SpeedProc();
    int AverageProd();
    int MaxLength();
    int MaxCustom();
    int Served();
    int Unserved();
    double AverageLen();
    double AverageTime();
    double AverageWorkCash();
    double AverageDownCash();
};
