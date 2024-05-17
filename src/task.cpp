// Copyright 2024 Mikhas3009

#include "task.h"

Product::Product(std::string name, double price) : Name(name), Price(price) {}

Cashbox::Cashbox(int timeToProcessProduct) :
    productProcessDelay(std::chrono::milliseconds(timeToProcessProduct)), IsBusy(false),
    ClientsServed(0), WorkTimeInMilliseconds(0), FreeTimeInMilliseconds(0),
    lastClientServed(std::chrono::steady_clock::now()) {}

void Cashbox::Serve(const Client& client) {
    IsBusy = true;
    auto start = std::chrono::steady_clock::now();
    FreeTimeInMilliseconds += std::chrono::duration_cast<std::chrono::milliseconds>(start - lastClientServed).count();

    for(const auto& product : client.Cart) {
        std::this_thread::sleep_for(productProcessDelay);
    }

    auto end = std::chrono::steady_clock::now();
    WorkTimeInMilliseconds += std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    lastClientServed = end;
    IsBusy = false;
}

// Implementation of Market class
Market::Market(int maxQueueLength,
               double clientsIntensity,
               int cashboxesCount,
               int timeToProcessProduct,
               double avgProductsInCart) :
    maxQueueLength(maxQueueLength), avgProductsInCart(avgProductsInCart),
    producerDelay(std::chrono::duration<double>(1 / clientsIntensity)),
    isLoggingEnabled(false), isWorking(false), ServedClients(0),
    RejectedClients(0), AvgQueueLength(0), AvgClientTime(0),
    AvgCashboxWorkTime(0), AvgCashboxFreeTime(0) {

    for(int i = 0; i < cashboxesCount; i++) {
        cashboxes.emplace_back(timeToProcessProduct);
    }
}

void Market::Simulate(int clientsCount) {
    Initialize();

    StartProducerThread(clientsCount);
    StartConsumerThread();

    JoinThreads();

    Finalize();
}

void Market::Initialize() {
    isWorking = true;
    AvgQueueLength = 0;
    AvgClientTime = 0;
    AvgCashboxWorkTime = 0;
    AvgCashboxFreeTime = 0;
}
Product Market::Products[5] = {
    Product("Eggs", 6),
    Product("Tomato", 8),
    Product("Potato", 2),
    Product("Chicken", 9),
    Product("Fish", 11)
};

void Market::StartProducerThread(int clientsCount) {
    producerThread = std::thread([this, clientsCount](){ Produce(clientsCount); });
}

void Market::StartConsumerThread() {
    consumerThread = std::thread([this](){ Consume(); });
}

void Market::JoinThreads() {
    producerThread.join();
    isWorking = false;
    consumerThread.join();
}

void Market::Finalize() {
    AvgQueueLength /= ServedClients;
    AvgClientTime /= ServedClients;

    for (const auto& cashbox : cashboxes) {
        AvgCashboxFreeTime += cashbox.FreeTimeInMilliseconds;
        AvgCashboxWorkTime += cashbox.WorkTimeInMilliseconds;
    }

    AvgCashboxFreeTime /= cashboxes.size();
    AvgCashboxWorkTime /= cashboxes.size();
}

void Market::Produce(int clientsCount) {
    for (int i = 0; i < clientsCount; i++) {
        std::this_thread::sleep_for(producerDelay);

        AvgQueueLength += clientsQueue.size();

        auto client = GenerateClient();
        client.Id = i;

        if(isLoggingEnabled) {
            std::cout << "Generated client " << client.Id
                << "(Products: " << client.Cart.size() << ")" << std::endl;
        }

        std::lock_guard<std::mutex> lock(queueLocker);
        if (clientsQueue.size() == maxQueueLength) {
            RejectedClients++;
            continue;
        }

        client.JoinedQueue = std::chrono::steady_clock::now();
        clientsQueue.push(client);
    }
}

void Market::Consume() {
    std::vector<std::thread> threadPool(cashboxes.size());

    while (isWorking || !clientsQueue.empty()) {
        for (int i = 0; i < cashboxes.size(); i++) {
            if(cashboxes[i].IsBusy) {
                continue;
            }

            if(threadPool[i].joinable()) {
                threadPool[i].join();
            }

            std::lock_guard<std::mutex> lock(queueLocker);
            if (!clientsQueue.empty()) {
                auto client = clientsQueue.front();
                clientsQueue.pop();
                threadPool[i] = std::thread(&Market::ProcessClient, this,
                                            std::cref(client), std::ref(cashboxes[i]));

                if(isLoggingEnabled) {
                    std::cout << "Servicing client " << client.Id
                        << "(Thread: " << threadPool[i].get_id() << ")" << std::endl;
                }
            }
        }
    }

    for (int i = 0; i < threadPool.size(); i++) {
        if(threadPool[i].joinable()) {
            threadPool[i].join();
        }
    }
}

void Market::ProcessClient(const Client& client, Cashbox& cashbox) {
    cashbox.Serve(client);

    std::lock_guard<std::mutex> lock(locker);
    ServedClients++;
    auto point = std::chrono::steady_clock::now();
    auto duration = point - client.JoinedQueue;
    AvgClientTime += std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
}

void Market::EnableLogging() {
    isLoggingEnabled = true;
}

void Market::DisableLogging() {
    isLoggingEnabled = false;
}

Client Market::GenerateClient() {
    int minProducts = 1;
    int maxProducts = std::ceil(avgProductsInCart * 2 - minProducts);
    int productsInCart = rand() % maxProducts + minProducts;

    Client client;
    for (int i = 0; i < productsInCart; i++) {
        client.Cart.push_back(Products[rand() % 5]);
    }

    return client;
}
