// Copyright 2024 Alexandr Kokorin
#include <iostream>
#include <algorithm>
#include <ctime>
#include "../include/task.h"

Shop::Shop(int numOfCashRegisters, double intensityOfCustomerFlow,
    double productProcessingSpeed, double AverageNumberOfGoods,
    int maxQueueLength) {
    this->numOfCashRegisters = numOfCashRegisters;
    this->buyerArrivalIntensity = intensityOfCustomerFlow;
    this->productProcessingSpeed = productProcessingSpeed;
    this->AverageNumberOfGoods = AverageNumberOfGoods;
    this->maxQueueLength = maxQueueLength;
    allTime = 0;
    countServedClients = 0;
    countUnservedClients = 0;
    queueLength = 0;
    timeInQueue = 0;
    serviceTime = 0;
    cashes = std::vector<Cash>(numOfCashRegisters);
    clientsQueue = std::queue<Client>();
    for (int i = 0; i < numOfCashRegisters; i++) {
        cashes[i] = Cash(i, productProcessingSpeed);
    }
}

Shop::~Shop() {}

void Shop::ServeTheClient(int index, Client client) {
    std::unique_lock<std::mutex> mu(myMutex);
    cashes[index].waitTime += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - cashes[index].startTime).count();
    cashes[index].workTime += client.numOfProducts * cashes[index].productProcessingSpeed;
    client.serviceTime = client.numOfProducts * cashes[index].productProcessingSpeed;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.numOfProducts * cashes[index].productProcessingSpeed)));
    cashes[index].startTime = std::chrono::system_clock::now();
    cashes[index].isFree = true;
    mu.unlock();
}

void Shop::StartWork() {
    allTimeStart = std::chrono::system_clock::now();

    srand(time(0));
    int time = 0;
    int nextCustomerTime = rand() % 10 + 1;
    int numberClients = 0;

    while (true) {
        if (numberClients < buyerArrivalIntensity) {
            time++;
            if (time == nextCustomerTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                numberClients++;
                Client client = Client(rand() % (int)(2 * AverageNumberOfGoods) + 1);
                if (clientsQueue.size() < maxQueueLength) {
                    clientsQueue.push(client);
                }
                else {
                    countUnservedClients++;
                }
                nextCustomerTime = time + rand() % 10 + 1;
            }
        }

        for (int i = 0; i < numOfCashRegisters; i++) {
            if ((clientsQueue.size() > 0) && (cashes[i].isFree == true)) {
                cashes[i].isFree = false;

                clientsQueue.front().waitTime = std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now() - clientsQueue.front().timeQueueStart).count();

                threads.push_back(new std::thread(&Shop::ServeTheClient, this, i, clientsQueue.front()));

                timeInQueue += clientsQueue.front().waitTime;
                serviceTime += clientsQueue.front().numOfProducts / productProcessingSpeed;
                queueLength += clientsQueue.size();

                countServedClients++;
                clientsQueue.pop();
            }
        }
        if (clientsQueue.empty() && numberClients == buyerArrivalIntensity) break;
    }
    for (auto& th : threads)
        th->join();

    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();
}

Cash::Cash() {
    index = 0;
    productProcessingSpeed = 0;
    workTime = 0;
    waitTime = 0;
    isFree = true;
    startTime = std::chrono::system_clock::now();
}

Cash::Cash(int index, double productProcessingSpeed) {
    this->index = index;
    this->productProcessingSpeed = productProcessingSpeed;
    workTime = 0;
    waitTime = 0;
    isFree = true;
    startTime = std::chrono::system_clock::now();
}


Client::Client(int numOfProducts) {
    this->numOfProducts = rand() % (2 * numOfProducts) + 1;
    timeQueueStart = std::chrono::system_clock::now();
    waitTime = 0;
    serviceTime = 0;
}

void Client::SetWaitTime() {
    waitTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - timeQueueStart).count();
}

double factorial(double n) {
    int fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
    }
    return fact;
}
