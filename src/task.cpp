// Copyright 2022 UNN-IASR
#include "task.h"

Buyer::Buyer(std::vector<int> _check, int _number) {
    check = _check;
    number = _number;
}

Supermarket::Supermarket(int _cashRegisterAmount, int _flowRate, int _processingSpeed, int _averageNumberOfProducts, int _maxQueueLength, int _maxBuyers) {
    cashRegisterAmount = _cashRegisterAmount;
    flowRate = _flowRate;
    processingSpeed = _processingSpeed;
    averageNumberOfProducts = _averageNumberOfProducts;
    maxQueueLength = _maxQueueLength;
    maxBuyers = _maxBuyers;
}

Buyer* Supermarket::createBuyer(int number) {
    int countProducts = std::rand() % (averageNumberOfProducts + 5) + (averageNumberOfProducts - 5);
    std::vector<int> check(countProducts);
    for (int i = 0; i < countProducts; i++) {
        check[i] = std::rand() % maxPrice + 1;
    }
    std::cout << "new buyer" << '\n';
    return new Buyer(check, number);
}

int Supermarket::getServedCustomers() {
    return servedCustomers;
}

int Supermarket::getNotServedCustomers() {
    return countBuyers - servedCustomers;
}

double Supermarket::getAverageQueueLength() {
    return averageQueueLength;
}

double Supermarket::getAverageTimeOfWaiting() {
    double result = 0;
    for (int i = 1; i <= averageQueueLength; i++) {
        result += i * static_cast<double>(averageNumberOfProducts * processingSpeed);
    }
    return result / averageQueueLength;
}

double Supermarket::getAverageCashRegisterWorkingTime() {
    return averageCashRegisterWorkingTime;
}
double Supermarket::getAverageCashRegisterNotWorkingTime() {
    return averageCashRegisterNotWorkingTime;
}
double Supermarket::refuseProbability() {
    return (refuseCount / countBuyers);
}

void Supermarket::run() {
    serveSupermarket();
    for (auto currentQueue : threads) {
        currentQueue->join();
    }
}

void Supermarket::serveBuyer(Buyer* _buyer, int _number) {
    for (auto i = 0; i < _buyer->check.size(); i++) {
        int time = std::rand() % processingSpeed;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
        std::unique_lock<std::mutex> unqLock(myMutex);
        std::cout << "Cash register " << std::this_thread::get_id() << ": buyer " << _number << " gets product " << i + 1 << '\n';
        unqLock.unlock();
        serviceTime += time;
        averageCashRegisterWorkingTime += processingSpeed / workingCashRegisterCount;
        averageCashRegisterNotWorkingTime += processingSpeed / (cashRegisterAmount - workingCashRegisterCount);
    }
    servedCustomers++;
}

void Supermarket::serveQueue(std::queue<Buyer*>* _buyers) {
    int number = 1;
    auto buyer = _buyers->front();
    _buyers->pop();
    serveBuyer(buyer, buyer->number);
    while (!buyerQueue.empty()) {
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
            std::unique_lock<std::mutex> unqLock(myMutex);
            unqLock.unlock();
            _buyers->pop();
            serveBuyer(buyer, buyer->number);
        }
    }
}

void Supermarket::serveSupermarket() {
    srand(time(nullptr));
    int countOfActive = 0;
    for (int i = 0; i < maxBuyers; i++) {
        countBuyers++;
        if (workingCashRegisterCount <= cashRegisterAmount) {
            auto ptrQ = new std::queue<Buyer*>;
            ptrQ->push(createBuyer(countBuyers));
            cashRegisterQueues.push_back(ptrQ);
            threads.push_back(new std::thread(&Supermarket::serveQueue, this, ptrQ));
            workingCashRegisterCount++;
        } else if (buyerQueue.size() < maxQueueLength) {
            buyerQueue.push(createBuyer(countBuyers));
        }

        averageQueueLength += buyerQueue.size();

    }
    averageQueueLength = averageQueueLength / countBuyers;
    averageCashRegisterWorkingTime = averageCashRegisterWorkingTime / countBuyers;
    averageCashRegisterNotWorkingTime = averageCashRegisterNotWorkingTime / countBuyers;

    serviceTime = serviceTime / countBuyers;
}
