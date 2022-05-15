// Copyright 2021 GHA Test Team
#include "task.h"
#include <iostream>
#include <thread>


Shop::Shop(int cashierCountTemp, int intencityTemp, int serviceTimeTemp, int averageProductsCountTemp, int maxQueueTemp, int customerCountMaxTemp) {
    intencity = intencityTemp;
    cashierCount = cashierCountTemp;
    customerCount = customerCountMaxTemp;
    serviceTime = serviceTimeTemp;
    averageProductsCount = averageProductsCountTemp;
    maxQueue = maxQueueTemp;

    servedCustomers = 0;
    unservedCustomers = 0;

    spareTimeCount = 0.0;
    workTimeCount = 0.0;
    avgTimeInQueue = 0.0;
    avgTimeInServing = 0.0;
    workTime = 0.0;

    cashierList = std::vector<Cashier>();
    customersQueue = std::queue<Customer*>();

    for (int i = 0; i < cashierCount; i++) {
        cashierList.push_back(Cashier());
        cashierList[i].spareTime = 0;
        cashierList[i].workTime = 0;
    }
}

void Shop::ServeQueue(int index) {
    while (servedCustomers + unservedCustomers < customerCount) {
        std::unique_lock<std::mutex> lock(mutex);
        if (customersQueue.size() > 0) {
            customersQueue.front()->OutsideQueue();

            cashierList[index].workTime = cashierList[index].workTime + (averageProductsCount * serviceTime);
            customersQueue.front()->outCashier(averageProductsCount * serviceTime);
            customersQueue.pop();
            servedCustomers++;
            std::cout << "Served - "<< servedCustomers<< " customers"<< std::endl;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(serviceTime*averageProductsCount));
        }
        else {
            cashierList[index].spareTime = cashierList[index].spareTime + 5;
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }
}

void Shop::InitShop() {
    for (int i = 0; i < cashierCount; i++) {
        cashierList[i].thread = std::thread(&Shop::ServeQueue, this, i);
    }

    int countCustomers = 0;
    double avgQueueLen = 0.0;
    std::vector<Customer> clients;

    while ((countCustomers < customerCount)) {
        
        avgQueueLen = avgQueueLen + customersQueue.size();
        std::unique_lock<std::mutex> lock(mutex);
        clients.push_back(Customer(averageProductsCount));
        if (customersQueue.size() < maxQueue)
        {
            clients.at(countCustomers).InsideQueue();
            customersQueue.push(&clients.at(countCustomers));
            lock.unlock();
        }
        else {
            unservedCustomers++;
            std::cout << "Could not serve - " << unservedCustomers << " customers" << std::endl;
            lock.unlock();
        }
        countCustomers++;
        std::this_thread::sleep_for(std::chrono::milliseconds(intencity));
    }

    for (int i = 0; i < cashierCount; i++) {
        cashierList[i].thread.join();
        spareTimeCount = spareTimeCount + cashierList[i].spareTime;
        workTimeCount = workTimeCount + cashierList[i].workTime;
    }

    for (int i = 0; i < customerCount; i++) {
        avgTimeInQueue = avgTimeInQueue + clients.at(i).timeQueue;
        avgTimeInServing = avgTimeInServing + clients.at(i).timeServed;
    }

    avgQueueLen = avgQueueLen / (double)countCustomers;
    avgTimeInQueue = avgTimeInQueue / (double)servedCustomers;
    avgTimeInServing = avgTimeInServing / (double)servedCustomers;
    workTimeCount = workTimeCount / (double)cashierCount;
    spareTimeCount = spareTimeCount / (double)cashierCount;
    workTime = workTimeCount + spareTimeCount;

    std::cout << "Time taken to serve all customers: " << workTime / 1000 << std::endl;
    std::cout << "Amount of served customers: " << servedCustomers << std::endl;
    std::cout << "Amount of unserved customers: " << unservedCustomers << std::endl;
    std::cout << "Average queue length: " << avgQueueLen << std::endl;
    std::cout << "Average time spent in queue: " << avgTimeInQueue / 1000 << std::endl;
    std::cout << "Average time spent with cashier: " << avgTimeInServing / 1000 << std::endl;
    std::cout << "Average cashier working time: " << workTimeCount / 1000 << std::endl;
    std::cout << "Average cashier spare time: " << spareTimeCount / 1000 << std::endl;
}

Customer::Customer(int averageProductsCount) {
    timeQueue = 0;
    timeServed = 0;
}

void Customer::InsideQueue() {
    startTime = std::chrono::system_clock::now();
}

void Customer::OutsideQueue() {
    timeQueue = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime).count();
}

void Customer::outCashier(int time) {
    timeServed = time;
}

double factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}