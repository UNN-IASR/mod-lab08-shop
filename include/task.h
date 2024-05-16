// Copyright 2024 Kapochkin Mikhail

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

struct Statistic {
    int appClients;
    int rejClients;
    int avgQueueLength;
    double avgClientWaitTime;
    double avgCashMachineWorkTime;
    double avgCashMachineWaitTime;
};

struct Client {
    int id;

    std::chrono::time_point<std::chrono::steady_clock> waitingStartTime;
    std::vector<std::string> cart;

    Client();
    Client(int ClientId, int numOfProducts);
};

class CashMachine {
private:
    bool isBusy;
    int delayTime;
    Client client;
    double workTime;
public:
    CashMachine(int _delay);
    bool IsBusy();
    double GetWorkTime();
    void ServClient(Client newClient);
};

class SuperMarket {
private:
    bool isWorking;
    int ClientsQueueLength;
    std::vector<CashMachine> cashMachinePool;
    std::queue<Client> clientsQueue;
    std::mutex queueMutex;
    std::chrono::time_point<std::chrono::steady_clock> workTime;

    bool QueueIsFull();
    double generateValue(double value);
    void StartCashMachineStream(CashMachine& cashMachine);
    Client CreateClient(int id, int avgNumOfProducts);
    void StartClientsStream(int numOfClients, double clientArrivalRate, int avgNumProducts);
public:
    Statistic statistic;
    SuperMarket(int numOfCashMachines, int sizeClientsQueue, int cashMachineSpeed);
    void SimulateWork(int numOfClients, double clientArriveRate, int avgNumProducts);
    // ������� ������������ ����������� � ��������� ������������ ������������
    SuperMarket(const SuperMarket&) = delete;
    SuperMarket& operator=(const SuperMarket&) = delete;

    // ���� ���������� ����������� ����������� � �������� ������������ ������������
    SuperMarket(SuperMarket&& other) noexcept
        : isWorking(other.isWorking),
        ClientsQueueLength(other.ClientsQueueLength),
        cashMachinePool(std::move(other.cashMachinePool)),
        clientsQueue(std::move(other.clientsQueue)),
        workTime(std::move(other.workTime)),
        statistic(std::move(other.statistic)) {
        // ����������: std::mutex �� ���������� � �� ������������, �������
        // ������ ������ SuperMarket ������ ����� ���� ����������� std::mutex.
        // �� �� ���������� queueMutex �� other � this, ��������� ��� ����������.
    }

    SuperMarket& operator=(SuperMarket&& other) noexcept {
        if (this != &other) {
            isWorking = other.isWorking;
            ClientsQueueLength = other.ClientsQueueLength;
            cashMachinePool = std::move(other.cashMachinePool);
            clientsQueue = std::move(other.clientsQueue);
            workTime = std::move(other.workTime);
            statistic = std::move(other.statistic);
            // ����� ��, std::mutex �� ������������, ��� ��� �� ���������� queueMutex.
        }
        return *this;
    }
};

