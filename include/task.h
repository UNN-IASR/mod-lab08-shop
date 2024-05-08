#pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <map>

class Store {
    private:
        int cashDeskCount;
        double customerIntensity;
        int productProcessingSpeed;
        int avgProductCount;
        int maxQueueLength;
        int countCustomer;
        bool isOpen;
        std::chrono::system_clock::time_point start;
        std::map<int, int> itemsInCartMap;
        std::queue<int> q;

        std::mutex mtx;

        void generateCustomer();
        int getRandomArrivalTime();
        int getRandomProductCount();
        void serveCustomer();
    public:
        Store(int _cashDeskCount, double _customerIntensity, int _productProcessingSpeed, int _avgProductCount, int _maxQueueLength, int _countCustomer);
        int acceptedCustomer = 0;
        int rejectedCustomer = 0;
        int sumQueueLength = 0;
        int busyCashierTime = 0;
        int storeTime = 0;
        int timeInQueue = 0;

        void simulate();
};