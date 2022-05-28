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

struct MarketStatistics
{
    int numberOfRequests;
    int numberOfProcessed;
    int numberOfFailed;
};

class Buyer {
 public:
    std::vector<int> cheak;
    explicit Buyer(std::vector<int> _cheak);
};

class Market {
 private:
    int maxPrice;
    int maxBuyers;
    int buyerIntensity;
    int averageNumberOfItems;
    int queueLen;
    int goodsService;
    int numberOfCashDesks;
    cashRegister staff;
    std::vector<std::queue<Buyer*>*> queues;
    std::mutex myMutex;
    MarketStatistics info = { 0, 0, 0 };
    bool allBuyersCame = false;
    double averageCheckoutTime = 0;
    double averageRestTime = 0;
    double averageQueueLength = 0;
    int numberOfCheak = 0;
    int sumOfQueueCount = 0;
    int numberOfOperatingCashDesks = 0;
 public:
    Market(int _maxPrice, int _maxBuyers, int _queueLen, int _averageNumberOfItems,
        int _goodsService, int _buyerIntensity, int _numberOfCashDesks);
    Buyer* createBuyer();
    void run();
    MarketStatistics getStatistics();
    void serveBuyer(Buyer* _buyer, int _number);
    void serveQueue(std::queue<Buyer*>* _buyers);
    void serveMarket();
    int getCountOfWorkingCashDesks();
    double getAverageQueueLength();
    double getAverageTimeOfWaiting();
    double getAverageCheckoutTime();
    double getAverageRestTime();
};
