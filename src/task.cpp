// Copyright 2022 UNN-IASR
#include <iostream>
#include "task.h"
#include <functional>

Buyer::Buyer(std::vector<int> _cheak) {
    cheak = _cheak;
}

Market::Market(int _maxPrice, int _maxBuyers, int _queueLen, int _averageNumberOfItems,
    int _goodsService, int _buyerIntensity, int _numberOfCashDesks) {
    maxPrice = _maxPrice;
    maxBuyers = _maxBuyers;
    queueLen = _queueLen;
    goodsService = _goodsService;
    buyerIntensity = _buyerIntensity;
    numberOfCashDesks = _numberOfCashDesks;
    averageNumberOfItems = _averageNumberOfItems;
}

MarketStatistics Market::getStatistics() {
    return info;
}

double Market::getAverageQueueLength() {
    return averageQueueLength;
}

double Market::getAverageTimeOfWaiting() {
    double result = 0;
    for (int i = 1; i <= averageQueueLength; i++) {
        result += i * static_cast<double>(averageNumberOfItems * goodsService);
    }
    return result / averageQueueLength;
}

double Market::getAverageCheckoutTime() {
    return averageCheckoutTime;
}

double Market::getAverageRestTime() {
    return averageRestTime;
}

int Market::getCountOfWorkingCashDesks() {
    return staff.size();
}

Buyer* Market::createBuyer() {
    std::vector<int> check(averageNumberOfItems);
    for (int i = 0; i < averageNumberOfItems; i++) {
        check[i] = std::rand() % maxPrice + 1;
    }
    return new Buyer(check);
}

void Market::run() {
    serveMarket();
    for (auto currentQueue : staff) {
        currentQueue->join();
    }
    averageQueueLength = sumOfQueueCount / numberOfCheak;
}

void Market::serveBuyer(Buyer* _buyer, int _number) {
    for (auto i = 0; i < _buyer->cheak.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(goodsService));
        std::unique_lock<std::mutex> unqLock(myMutex);
        averageCheckoutTime += goodsService * static_cast<std::int64_t>(numberOfOperatingCashDesks)
            / static_cast<double>(numberOfCashDesks);
        averageRestTime += goodsService * (numberOfCashDesks - static_cast<std::int64_t>(numberOfOperatingCashDesks))
            / static_cast<double>(numberOfCashDesks);
        std::cout << "Cashbox" << std::this_thread::get_id()
            << " : buyer " << _number << " gets product" << i + 1 << "\n";
        unqLock.unlock();
    }
    info.numberOfProcessed++;
}

void Market::serveQueue(std::queue<Buyer*>* _buyers) {
    int number = 1;
    while (!allBuyersCame) {
        if (!_buyers->empty()) {
            int countElements = 0;
            int iteration = 0;
            std::queue<int>* cameBefore = new std::queue<int>();
            while (!_buyers->empty()) {
                auto buyer = _buyers->front();
                serveBuyer(buyer, number);
                _buyers->pop();
                countElements++;
                number++;
                iteration++;
            }
            std::unique_lock<std::mutex> unqLock(myMutex);
            sumOfQueueCount += countElements;
            numberOfCheak++;
            unqLock.unlock();
        }
    }
}

void Market::serveMarket() {
    srand(time(nullptr));
    int countOfActive = 0;
    for (int i = 0; i < maxBuyers; i++) {
        info.numberOfRequests++;
        numberOfOperatingCashDesks = 0;
        for (auto it = queues.begin(); it != queues.end(); it++) {
            if ((*it)->size() > 0) {
                numberOfOperatingCashDesks++;
            }
        }
        bool findQueue = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(buyerIntensity));
        for (auto it = queues.begin(); it != queues.end(); it++) {
            if ((*it)->size() < queueLen) {
                (*it)->push(createBuyer());
                findQueue = true;
                break;
            }
        }
        if (!findQueue) {
            if (countOfActive < numberOfCashDesks) {
                countOfActive++;
                auto ptrQ = new std::queue <Buyer*>;
                ptrQ->push(createBuyer());
                queues.push_back(ptrQ);
                staff.push_back(new std::thread(&Market::serveQueue, this, ptrQ));
            }
            else {
                info.numberOfFailed++;
            }
        }
    }
    allBuyersCame = true;
}