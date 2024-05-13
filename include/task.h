// Copyright 2024 Artyom Ignatyev

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_
#endif  // INCLUDE_TEXTGEN_H_

#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <queue>
#include <random>
#include <mutex>
#include <algorithm>

struct Statistics {
    int servedClients;
    int rejectedClients;
    int avgQueueLength;
    double avgClientWaitingTime;
    double avgCashRegisterWorkTime;
    double averageCashRegisterDowntime;
};

struct Client {
    int id;
    std::vector<std::string> cart;
    std::chrono::time_point<std::chrono::steady_clock> waitingStartTime;
    
    Client();
    Client(int _id, int numProducts);
};

class CashRegister {
private:
    bool isBusy;
    int delay;
    Client client;
    double workTime;
public:
    CashRegister(int _delay);
    bool IsBusy();
    double GetWorkTime();
    void ServClient(Client newClient);
};

class Shop {
private:
    bool isWork; 
    int maxLenClientsQueue;
    std::vector<CashRegister> cashRegisterPool;
    std::queue<Client> clientsQueue;
    std::mutex queueMutex;
    std::chrono::time_point<std::chrono::steady_clock> workTime;

    bool QueueIsFull();
    double generatePoissonValue(double value);
    Client CreateClient(int id, int avgNumProducts);
    void RunClientsStream(int numClients, double clientArrivalRate, int avgNumProducts);
    void RunCashRegisterStream(CashRegister& cashRegister);
public:
    Statistics statistics;
    Shop(int numCashRegisters, int sizeClientsQueue, int cashRegisterSpeed);
    void Simulate(int numClients, double clientArrivalRate, int avgNumProducts);
};