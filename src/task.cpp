// Copyright 2022 UNN-IASR
#include <iostream>
#include "task.h"
#include <functional>

Buyer::Buyer(std::vector<int> _check) {
    check = _check;
}

Market::Market(int _maxPrice, int _maxBuyers, int _queueLen, int _averageNumberOfItems,
    int _goodsService, int _buyerIntensity, int _numberOfCashRegisters) 
    
    :
    maxPrice(_maxPrice),
    maxBuyers(_maxBuyers),
    queueLen(_queueLen),
    goodsService(_goodsService),
    buyerIntensity(_buyerIntensity),
    numberOfCashRegisters(_numberOfCashRegisters),
    averageNumberOfItems(_averageNumberOfItems)

    { }

void Market::serveBuyer(Buyer* _buyer, int _number) {
    for (auto i = 0; i < _buyer->check.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(goodsService));
        std::unique_lock<std::mutex> unqLock(market_lock);
        averageCheckoutTime += goodsService * static_cast<std::int64_t>(numberOfWorkingCashRegisters)
            / static_cast<double>(numberOfCashRegisters);
        averageIdleTime += goodsService * (numberOfCashRegisters - static_cast<std::int64_t>(numberOfWorkingCashRegisters))
            / static_cast<double>(numberOfCashRegisters);
        std::cout << "Касса " << std::this_thread::get_id()
            << ": покупатель " << _number << " получает товар " << i + 1 << "\n";
        unqLock.unlock();
    }
    stats.processed_count++;
}

void Market::serveQueue(std::queue<Buyer*>* _buyers) {
    int number = 1;
    while (!allBuyersDone) {
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
            std::unique_lock<std::mutex> unqLock(market_lock);
            sumOfQueueLengths += countElements;
            numberOfChecks++;
            unqLock.unlock();
        }
    }
}

void Market::serveMarket() {
    srand(time(nullptr));
    int countOfActive = 0;
    for (int i = 0; i < maxBuyers; i++) {
        stats.request_count++;
        numberOfWorkingCashRegisters = 0;
        for (auto it = queues.begin(); it != queues.end(); it++) {
            if ((*it)->size() > 0) {
                numberOfWorkingCashRegisters++;
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
            if (countOfActive < numberOfCashRegisters) {
                countOfActive++;
                auto ptrQ = new std::queue <Buyer*>;
                ptrQ->push(createBuyer());
                queues.push_back(ptrQ);
                staff.push_back(new std::thread(&Market::serveQueue, this, ptrQ));
            }
            else {
                stats.failed_count++;
            }
        }
    }
    allBuyersDone = true;
}

double Market::getAverageWaitTime() {
    double result = 0;
    for (int i = 1; i <= averageQueueLength; i++) {
        result += i * static_cast<double>(averageNumberOfItems * goodsService);
    }
    return result / averageQueueLength;
}

MarketStats Market::getStats() {
    return stats;
}

double Market::getAverageQueueLength() {
    return averageQueueLength;
}

double Market::getAverageIdleTime() {
    return averageIdleTime;
}

double Market::getAverageCheckoutTime() {
    return averageCheckoutTime;
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
    averageQueueLength = sumOfQueueLengths / numberOfChecks;
}