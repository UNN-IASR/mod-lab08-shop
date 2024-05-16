
// Copyright 2024 Kapochkin Mikhail

#include "../include/task.h"
//Конструкторы клиентов
Client::Client() {}
Client::Client(int _id, int numProducts) : id(_id), cart(numProducts) {}
//Конструктор кассы
CashMachine::CashMachine(int _delay) : isBusy(false), delayTime(_delay) {}
//Проверка занятости кассы
bool CashMachine::IsBusy() { return isBusy; }
//Вовращаем общее рабочее время
double CashMachine::GetWorkTime() { return workTime; }
//Симулируем оплату на кассе
void CashMachine::ServClient(Client newClient) {
    isBusy = true;
    auto startTime = std::chrono::steady_clock::now();
    client = newClient;

    std::this_thread::sleep_for(std::chrono::milliseconds(delayTime * client.cart.size()));

    auto endTime = std::chrono::steady_clock::now();
    workTime += (endTime - startTime).count();
    isBusy = false;
}
//Конструктор магазина
SuperMarket::SuperMarket(int numCashRegisters, int sizeClientsQueue, int cashRegisterDelay) : isWorking(false) {
    ClientsQueueLength = sizeClientsQueue;
    cashMachinePool = std::vector<CashMachine>(numCashRegisters, CashMachine(cashRegisterDelay));
    statistic = { 0, 0, 0, 0.0, 0.0, 0.0 };
}
//Создаём клиента
Client SuperMarket::CreateClient(int id, int avgNumProducts) {
    int numProducts = generateValue(avgNumProducts);
    return Client(id, avgNumProducts);
}
//Проверка полноты очереди
bool SuperMarket::QueueIsFull() { return clientsQueue.size() == ClientsQueueLength; }
//Поток обработки клиента
void SuperMarket::StartClientsStream(int numClients, double clientArrivalRate, int avgNumProducts) {

    for (int i = 0; i < numClients; i++) {
        auto delay = std::chrono::milliseconds(1000 / (int)generateValue(clientArrivalRate));
        std::this_thread::sleep_for(delay);

        Client newClient = CreateClient(i, avgNumProducts);
        queueMutex.lock();
        statistic.avgQueueLength += clientsQueue.size();
        if (!QueueIsFull()) {
            newClient.waitingStartTime = std::chrono::steady_clock::now();
            clientsQueue.push(newClient);
            queueMutex.unlock();
            continue;
        }
        queueMutex.unlock();
        statistic.rejClients++;
    }
}
//Поток для кассы
void SuperMarket::StartCashMachineStream(CashMachine& cashMachine) {
    while (isWorking || !clientsQueue.empty()) {
        if (!cashMachine.IsBusy()) {
            std::unique_lock<std::mutex> locker(queueMutex);
            if (!clientsQueue.empty()) {
                Client client = clientsQueue.front();
                clientsQueue.pop();
                locker.unlock();
                cashMachine.ServClient(client);
                statistic.avgClientWaitTime = (std::chrono::steady_clock::now() - client.waitingStartTime).count();
                statistic.appClients++;
            }
            else {
                locker.unlock();
            }
        }
    }
}
//Генератор числа
double SuperMarket::generateValue(double value) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> dist(value);
    return dist(gen);
}
//Симулируем работу, фиксируя статистику
void SuperMarket::SimulateWork(int numClients, double clientArrivalRate, int avgNumProducts) {
    isWorking = true;
    workTime = std::chrono::steady_clock::now();
    std::thread clientsThread(&SuperMarket::StartClientsStream, this, numClients, clientArrivalRate, avgNumProducts);

    std::vector<std::thread> cashRegisterThreads;
    for (auto& cashRegister : cashMachinePool) {
        cashRegisterThreads.push_back(std::thread(&SuperMarket::StartCashMachineStream, this, std::ref(cashRegister)));
    }

    clientsThread.join();
    isWorking = false;

    for (auto& thread : cashRegisterThreads) {
        thread.join();
    }

    statistic.avgClientWaitTime /= statistic.appClients;
    for (int i = 0; i < cashMachinePool.size(); i++) {
        statistic.avgCashMachineWorkTime += cashMachinePool[i].GetWorkTime();
    }
    statistic.avgCashMachineWorkTime /= cashMachinePool.size();
    statistic.avgCashMachineWaitTime = (std::chrono::steady_clock::now() - workTime).count() - statistic.avgCashMachineWorkTime;
    statistic.avgQueueLength /= statistic.appClients + statistic.rejClients;
}