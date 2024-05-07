// Copyright 2024 Danil Volkov

#include <iostream>
#include <thread>
#include <random>
#include "../include/task.h"

Shop::Shop(int numThreads, int maxQueueLen, double speedProductCash) : maxQueueLength(maxQueueLen), speedProductCash(speedProductCash) {
    pool.resize(numThreads);
}

Shop::~Shop() {
    stop = true;
    cv.notify_all();
    for (auto& thread : pool) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void Shop::proc(ProcEventArgs args) {
    std::unique_lock<std::mutex> lock(threadLock);
    std::cout << "���������� " << args.id << " ����� � �������" << std::endl;
    args.enterQueueTime = std::chrono::steady_clock::now(); // ��������� ����� ����� � �������
    requestCount++;

    if (requests.size() < maxQueueLength) {
        requests.push(args);
        totalQueueLength += requests.size();
        cv.notify_one();
    }
    else {
        rejectedCount++;
        std::cout << "������� �� ����� �����, ���������� " << args.id << " ������" << std::endl;
    }
}

void Shop::answer() {
    while (true) {
        std::unique_lock<std::mutex> lock(threadLock);
        cv.wait(lock, [this] { return !requests.empty() || stop || allCustomersProcessed; });
        if (!requests.empty()) {
            ProcEventArgs args = requests.front();
            requests.pop();
            args.enterCashierTime = std::chrono::steady_clock::now(); // ��������� ����� ������ ������������ �� �����
            processedCustomers.push_back(args); // ��������� ������������� �������
            totalQueueLength -= 1;
            lock.unlock();
            std::cout << "���������� " << args.id << " ����� ������������" << std::endl;
            processedCount++;
            for (int product : args.cart) {
                std::cout << "���������� " << args.id << " ������������ ����� " << product << std::endl;
                // ������������� ��������� ������
                std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / speedProductCash)));
            }
            exitCashierTime += std::chrono::duration<double>(std::chrono::steady_clock::now().time_since_epoch()).count();
            std::cout << "���������� " << args.id << " �������� ������������" << std::endl;
        }
        else {
            if (allCustomersProcessed) {
                // ������� �� �����, ���� ��� ������� ���������� � ������� �����
                break; 
            }
        }
    }
}

double Shop::getAverageQueueTime() const {
    double totalQueueTime = 0;
    for (const ProcEventArgs& args : processedCustomers) {
        totalQueueTime += std::chrono::duration<double>(args.enterCashierTime - args.enterQueueTime).count();
    }
    return totalQueueTime / processedCustomers.size();
}

double Shop::getAverageCashierTime() const {
    double totalCashierTime = 0;
    for (const ProcEventArgs& args : processedCustomers) {
        totalCashierTime += std::chrono::duration<double>(args.enterCashierTime.time_since_epoch()).count();
    }
    return (exitCashierTime - totalCashierTime) / processedCustomers.size();
}

double Shop::getAverageQueueLength() const {
    return static_cast<double>(totalQueueLength) / static_cast<double>(maxQueueLength);
}

void Shop::setAllCustomersProcessed() {
    allCustomersProcessed = true;
    // ���������� ��� ������ �����, ��� ��� ������� ����������
    cv.notify_all(); 
}

bool Shop::isAllCustomersProcessed() const {
    return allCustomersProcessed;
}

int Shop::getRequestCount() {
    return requestCount;
}

int Shop::getProcessedCount() {
    return processedCount;
}

int Shop::getRejectedCount() {
    return rejectedCount;
}

Client::Client(Shop* shop, int averageItems) : shop(shop) {
    int lowerBound = std::max(1, averageItems - 2);
    distribution = std::uniform_int_distribution<int>(lowerBound, averageItems + 2);
}

void Client::send(int id) {
    ProcEventArgs args;
    args.id = id;
    int numItems = distribution(generator); // ��������� ���������� ������� � �������
    std::cout << "���������� " << id << " ������ �� ���������� �������� � �������: ";
    for (int i = 0; i < numItems; ++i) {
        args.cart.push_back(i + 1); // ��������� ������ � �������� �� 1 �� numItems
        std::cout << i + 1 << " ";
    }
    std::cout << std::endl;
    onProc(args);
}

void Client::onProc(ProcEventArgs& e) {
    shop->proc(e);
}

unsigned long long factorial(int n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}
