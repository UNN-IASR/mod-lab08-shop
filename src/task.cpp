// Copyright 2022 Dacyto
#include "task.h"

Client::Client(std::vector<int> receipt) {
    this->receipt = receipt;
}

Shop::Shop(int cashboxAmount, int clientsIntensity, int processingSpeed, int averageQuantity, int maximumQueue) {
    this->cashboxAmount = cashboxAmount;
    this->clientsIntensity = clientsIntensity;
    this->processingSpeed = processingSpeed;
    this->averageQuantity = averageQuantity;
    this->maximumQueue = maximumQueue;
}

Client* Shop::getClient() {
    std::vector<int> receipt(averageQuantity);

    for (int i = 0; i < averageQuantity; i++) {
        receipt[i] = std::rand() % 40 + 1;
    }

    return new Client(receipt);
}

void Shop::startShopping() {
    serveShop();

    for (auto currentQueue : workingCashboxes) {
        currentQueue->join();
    }

    averageQueueLength = static_cast<double>(numberOfQueues / numberOfReceipts);
}

void Shop::serveClient(Client* client, int number) {
    for (auto i = 0; i < client->receipt.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(processingSpeed));
        std::unique_lock<std::mutex> lock(mutex);

        averageCashboxWorktime += static_cast<double>(processingSpeed * static_cast<std::int64_t>(numberOfWorkingCashboxes) / static_cast<double>(cashboxAmount));
        averageCashboxDowntime += static_cast<double>(processingSpeed * (cashboxAmount - static_cast<std::int64_t>(numberOfWorkingCashboxes)) / static_cast<double>(cashboxAmount));

        std::cout << "Cashbox #" << std::this_thread::get_id() << ". Client #" << number << " with product #" << i + 1 << "\n";

        lock.unlock();
    }

    servedClients++;
}

void Shop::serveQueue(std::queue<Client*>* client) {
    int amountServerClients = 1;
    while (!isFinished) {
        if (!client->empty()) {
            int count = 0;
            std::queue<int>* prevs = new std::queue<int>();

            while (!client->empty()) {
                auto newClient = client->front();
                serveClient(newClient, amountServerClients);
                client->pop();
                count++;
                amountServerClients++;
            }

            std::unique_lock<std::mutex> lock(mutex);
            numberOfQueues += count;
            numberOfReceipts++;
            lock.unlock();
        }
    }
}

void Shop::serveShop() {
    srand(time(NULL));
    int currentQueues = 0;

    for (int i = 0; i < maximumQueue; i++) {
        numberOfWorkingCashboxes = 0;

        for (auto it = queues.begin(); it != queues.end(); it++) {
            if ((*it)->size() > 0) {
                numberOfWorkingCashboxes++;
            }
        }

        bool isFree = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(clientsIntensity));

        for (auto it = queues.begin(); it != queues.end(); it++) {
            if ((*it)->size() < queueLength) {
                (*it)->push(getClient());
                isFree = true;
                break;
            }
        }

        if (!isFree) {
            if (currentQueues < cashboxAmount) {
                currentQueues++;
                auto nextQueue = new std::queue <Client*>;
                nextQueue->push(getClient());
                queues.push_back(nextQueue);
                workingCashboxes.push_back(new std::thread(&Shop::serveQueue, this, nextQueue));
            }
            else {
                notServedClients++;
            }
        }
    }

    isFinished = true;
}


int Shop::getAmountOfServedClients() {
    return servedClients;
}

int Shop::getAmountOfNotServedClients() {
    return notServedClients;
}

double Shop::getAverageQueueLength() {
    return averageQueueLength;
}

double Shop::getAverageClientTime() {
    double result = 0;

    for (int i = 1; i <= averageQueueLength; i++) {
        result += i * static_cast<double>(averageQuantity * processingSpeed);
    }

    return result / averageQueueLength;
}

double Shop::getAverageCashboxWorktime() {
    return averageCashboxWorktime;
}

double Shop::getAverageCashboxDowntime() {
    return averageCashboxDowntime;
}

double Shop::getPrej() {
    double p = static_cast<double>(clientsIntensity) / static_cast<double>(processingSpeed);

    double P0 = 1.0;
    for (int i = 1; i <= cashboxAmount; i++) {
        P0 = P0 + pow(p, i) / factorial(i);
    }

    for (int i = cashboxAmount + 1; i < cashboxAmount + maximumQueue; i++) {
        P0 = P0 + pow(p, i) / (factorial(cashboxAmount) * pow(cashboxAmount, i - cashboxAmount));
    }

    P0 = 1.0 / P0;

    double Prej = P0 * pow(p, (cashboxAmount + maximumQueue)) / static_cast<double>(pow(cashboxAmount, maximumQueue) * factorial(cashboxAmount));
    return Prej;
}

double Shop::getQ() {
    double Prej = getPrej();
    return 1.0 - Prej;
}

double Shop::getA() {
    double Q = getQ();
    return static_cast<double>(processingSpeed) * Q;
}

double Shop::factorial(double n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}
