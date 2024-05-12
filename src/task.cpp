// Copyright 2024 Yuriy Nazarov
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
        avgQueueLength = 0;
        avgTimeInQueue = 0;
        avgServiceTime = 0;
        avgWorkCashTime = 0;
        avgWaitCashTime = 0;
        cashes = std::vector<Cash>(numOfCashRegisters);
        clientsQueue = std::queue<Client>();
        for (int i =0; i < numOfCashRegisters; i++){
            cashes[i] = Cash(i, productProcessingSpeed);
        }
}

Shop::~Shop(){}

void Shop::ServeTheClient(int index, Client client) {
    std::unique_lock<std::mutex> mu(myMutex);
    cashes[index].waitTime += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - cashes[index].startTime).count();
    cashes[index].workTime += client.numOfProducts*cashes[index].productProcessingSpeed;
    client.serviceTime = client.numOfProducts*cashes[index].productProcessingSpeed;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.numOfProducts*cashes[index].productProcessingSpeed)));
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
                } else {
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

                //cобираем статистику
                avgTimeInQueue += clientsQueue.front().waitTime;
                avgServiceTime += clientsQueue.front().numOfProducts / productProcessingSpeed;
                avgQueueLength += clientsQueue.size();

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

void Shop::CalculateStatistics() {
    avgWorkCashTime = 0;
    avgWaitCashTime = 0;

    for (int i = 0; i < numOfCashRegisters; i++) {
        avgWorkCashTime += cashes[i].workTime;
        avgWaitCashTime += cashes[i].waitTime;
    }

    avgQueueLength = avgQueueLength / buyerArrivalIntensity;
    avgTimeInQueue = avgTimeInQueue / buyerArrivalIntensity;
    avgServiceTime = avgServiceTime / buyerArrivalIntensity;

    avgWorkCashTime = avgWorkCashTime / numOfCashRegisters;
    avgWaitCashTime = avgWaitCashTime / numOfCashRegisters;

    double la = buyerArrivalIntensity / allTime;
    double mu = (double)countServedClients / allTime;
    double p = la / mu;

    //практические данные
    double P = countUnservedClients / buyerArrivalIntensity;
    double Q = 1.0 - P;
    double A = la * Q;

    pPn = P;
    pQ = Q;
    pA = A;

    //теоретические расчеты
    double P0 = 1.0;
    for (int i = 1; i <= numOfCashRegisters; i++)
        P0 = P0 + pow(p, i) / factorial(i);

    for (int i = numOfCashRegisters + 1; i < numOfCashRegisters + maxQueueLength; i++)
        P0 = P0 + pow(p, i) / (factorial(numOfCashRegisters) * pow(numOfCashRegisters, i - numOfCashRegisters));

    P0 = 1.0 / P0;
    P = P0 * pow(p, (numOfCashRegisters + maxQueueLength)) / (double)(pow(numOfCashRegisters, maxQueueLength) * factorial(numOfCashRegisters));
    Q = 1.0 - P;
    A = la * Q;
    tPn = P;
    tQ = Q;
    tA = A;
}

Cash::Cash(){
    index = 0;
    productProcessingSpeed = 0;
    workTime = 0;
    waitTime = 0;
    isFree = true;
    startTime = std::chrono::system_clock::now();
}

Cash::Cash(int index, double productProcessingSpeed){
    this->index = index;
    this->productProcessingSpeed = productProcessingSpeed;
    workTime = 0;
    waitTime = 0;
    isFree = true;
    startTime = std::chrono::system_clock::now();
}


Client::Client(int numOfProducts){
    this->numOfProducts = rand() % (2 * numOfProducts) + 1;
    timeQueueStart = std::chrono::system_clock::now();
    waitTime = 0;
    serviceTime = 0;
}

void Client::SetWaitTime(){
    waitTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - timeQueueStart).count();
}

double factorial(double n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}