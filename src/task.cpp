// Copyright 2024 Artyom Ignatyev

#include "../include/task.h"

Client::Client() {}
Client::Client(int _id, int numProducts) : id(_id), cart(numProducts) {}

CashRegister::CashRegister(int _delay) : isBusy(false), delay(_delay) {}

bool CashRegister::IsBusy() { return isBusy; }

double CashRegister::GetWorkTime() { return workTime; }

void CashRegister::ServClient(Client newClient) {
    isBusy = true;
    auto startTime = std::chrono::steady_clock::now();
    client = newClient;

    std::this_thread::sleep_for(std::chrono::milliseconds(delay * client.cart.size()));

    auto endTime = std::chrono::steady_clock::now();
    workTime += (endTime - startTime).count();
    isBusy = false;
}

Shop::Shop(int numCashRegisters, int sizeClientsQueue, int cashRegisterDelay) : isWork(false) {
    maxLenClientsQueue = sizeClientsQueue;
    cashRegisterPool = std::vector<CashRegister>(numCashRegisters, CashRegister(cashRegisterDelay));
    statistics = {0, 0, 0, 0.0, 0.0, 0.0};
}

void Shop::Simulate(int numClients, double clientArrivalRate, int avgNumProducts) {
    isWork = true;
    workTime = std::chrono::steady_clock::now();
    std::thread clientsThread(&Shop::RunClientsStream, this, numClients, clientArrivalRate, avgNumProducts);

    std::vector<std::thread> cashRegisterThreads;
    for (auto& cashRegister : cashRegisterPool) {
        cashRegisterThreads.push_back(std::thread(&Shop::RunCashRegisterStream, this, std::ref(cashRegister)));
    }

    clientsThread.join();
    isWork = false;

    for (auto& thread : cashRegisterThreads) {
        thread.join();
    }

    statistics.avgClientWaitingTime /= statistics.servedClients;
    for (int i = 0; i < cashRegisterPool.size(); i++) {
        statistics.avgCashRegisterWorkTime += cashRegisterPool[i].GetWorkTime();
    }
    statistics.avgCashRegisterWorkTime /= cashRegisterPool.size();
    statistics.averageCashRegisterDowntime = (std::chrono::steady_clock::now() - workTime).count() - statistics.avgCashRegisterWorkTime;
    statistics.avgQueueLength /= statistics.servedClients + statistics.rejectedClients;
}

bool Shop::QueueIsFull() { return clientsQueue.size() == maxLenClientsQueue; }

Client Shop::CreateClient(int id, int avgNumProducts) {
    int numProducts = generatePoissonValue(avgNumProducts);
    return Client(id, avgNumProducts);
}

void Shop::RunClientsStream(int numClients, double clientArrivalRate, int avgNumProducts){

    for (int i = 0; i < numClients; i++) {
        auto delay = std::chrono::milliseconds(1000 / (int)generatePoissonValue(clientArrivalRate));
        std::this_thread::sleep_for(delay);

        Client newClient = CreateClient(i, avgNumProducts);
        queueMutex.lock();
        statistics.avgQueueLength += clientsQueue.size();
        if (!QueueIsFull()) {
            newClient.waitingStartTime = std::chrono::steady_clock::now();
            clientsQueue.push(newClient);
            queueMutex.unlock();
            continue;
        }
        queueMutex.unlock();
        statistics.rejectedClients++;
    }
}

void Shop::RunCashRegisterStream(CashRegister& cashRegister) {
    while (isWork || !clientsQueue.empty()) {
        if (!cashRegister.IsBusy()) {
            std::unique_lock<std::mutex> locker(queueMutex);
            if (!clientsQueue.empty()) {
                Client client = clientsQueue.front();
                clientsQueue.pop();
                locker.unlock();
                cashRegister.ServClient(client);
                statistics.avgClientWaitingTime = (std::chrono::steady_clock::now() - client.waitingStartTime).count();
                statistics.servedClients++;
            }
            else {
                locker.unlock();
            }
        }
    }
}

double Shop::generatePoissonValue(double value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> dist(value);
    return dist(gen);
}