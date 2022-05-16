// Copyright 2022 UNN-IASR
#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <mutex>
#include <list>
#include <thread>
#include <condition_variable>

typedef std::vector<std::thread*> cashRegister;

struct MarketStats
{
    int processed_count;
    int failed_count;
    int request_count;
};

class Buyer {
public:
    std::vector<int> check;
    explicit Buyer(std::vector<int> _check);
};

class Market {
private:
    int averageNumberOfItems;
    int queueLen;
    int numberOfCashRegisters;
    int maxPrice;
    cashRegister staff;
    std::vector<std::queue<Buyer*>*> queues;
    std::mutex market_lock;
    bool allBuyersDone = false;
    int goodsService;
    int buyerIntensity;
    int maxBuyers;
    MarketStats stats = { 0 };
    int numberOfChecks = 0;
    int sumOfQueueLengths = 0;
    int numberOfWorkingCashRegisters = 0;
    double averageCheckoutTime = 0;
    double averageIdleTime = 0;
    double averageQueueLength = 0;
public:

    double getAverageWaitTime();
    double getAverageIdleTime();
    double getAverageCheckoutTime();
    Market(int _maxPrice, int _maxBuyers, int _queueLen, int _averageNumberOfItems,
        int _goodsService, int _buyerIntensity, int _numberOfCashRegisters);
    Buyer* createBuyer();
    void run();
    double getAverageQueueLength();
    int getCountOfWorkingCashDesks();
    MarketStats getStats();
    void serveBuyer(Buyer* _buyer, int _number);
    void serveQueue(std::queue<Buyer*>* _buyers);
    void serveMarket();
};