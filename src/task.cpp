// Copyright 2024 Alexander Sidelnikov

#include "task.h"

Product Market::Products[5] = {
    Product("Apple", 10),
    Product("Bread", 3),
    Product("Milk", 4),
    Product("Cheese", 7),
    Product("Butter", 5)
};

Product::Product(std::string name, double price) {
    Name = name;
    Price = price;
}

Market::Market(int maxQueueLength,
               double clientsIntensity,
               int chashboxesCount,
               int timeToProcessProduct,
               double avgProductsInCart) {
    this->maxQueueLength = maxQueueLength;
    for(int i = 0; i < chashboxesCount; i++) {
        cashboxes.push_back(Cashbox(timeToProcessProduct));
    }
    this->avgProductsInCart = avgProductsInCart;
    producerDelay = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double>(1 / clientsIntensity)
    );
    isLoggingEnabled = false;
}

void Market::Simulate(int clientsCount) {
    isWorking = true;
    std::thread producerThread(Market::Produce, this, clientsCount);
    std::thread consumerThread(Consume, this);

    producerThread.join();
    isWorking = false;
    consumerThread.join();
    
    AvgQueueLength /= clientsCount;
    AvgClientTime /= clientsCount;
    
    for (int i = 0; i < cashboxes.size(); i++) {
        AvgCashboxFreeTime += cashboxes[i].FreeTimeInMilliseconds;
        AvgCashboxWorkTime += cashboxes[i].WorkTimeInMilliseconds;
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
            std::cout << "Сгенерирован клиент " << client.Id
            << "(Продуктов: " << client.Cart.size() << ")"
                << std::endl;
        }
        queueLocker.lock();
        if (clientsQueue.size() == maxQueueLength) {
            RejectedClients++;
            queueLocker.unlock();
            continue;
        }
        
        client.joinedQueue = std::chrono::steady_clock::now();
        clientsQueue.push(client);
        queueLocker.unlock();
    }
}

Client Market::GenerateClient() {
    int minProducts = 1;
    int maxProducts = std::ceil(avgProductsInCart*2 - minProducts);
    int productsInCart = rand() % maxProducts + minProducts;
    
    Client client;
    for (int i = 0; i < productsInCart; i++) {
        client.Cart.push_back(Products[rand() % 5]);
    }

    return client;
}

void Market::Consume() {
    std::vector<std::thread> threadPool(cashboxes.size());

    while (isWorking || !clientsQueue.empty()) {
        for (int i = 0; i < cashboxes.size(); i++) {
            if(cashboxes[i].IsBusy) {
                continue;
            }
            if(threadPool[i].joinable()){
                threadPool[i].join();
            }
            queueLocker.lock();
            if (!clientsQueue.empty()) {
                auto client = clientsQueue.front();
                clientsQueue.pop();
                threadPool[i] = std::thread {Market::ProcessClient, 
                                                this,
                                                std::ref(client), 
                                                std::ref(cashboxes[i])};
                if(isLoggingEnabled) {
                    std::cout << "Обслуживание клиента " 
                        << client.Id << "(Поток: " 
                        << threadPool[i].get_id() << ")" << std::endl;
                }
            }
            queueLocker.unlock();
        }
    }

    for (int i = 0; i < threadPool.size(); i++) {
        if(threadPool[i].joinable())
            threadPool[i].join();
    }
}

void Market::ProcessClient(const Client& client, Cashbox& cashbox) {
    cashbox.Serve(client);
    
    locker.lock();
    ServedClients++;
    auto point = std::chrono::steady_clock::now();
    auto duration = point - client.joinedQueue;
    AvgClientTime += duration.count();
    locker.unlock();
}

void Market::EnableLogging() {
    isLoggingEnabled = true;
}

void Market::DisableLogging() {
    isLoggingEnabled = false;
}

Cashbox::Cashbox(int timeToProcessProduct) {
    productProcessDelay = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double>((double)timeToProcessProduct / 1000));
    lastClientServed = std::chrono::steady_clock::now();
    IsBusy = false;
}

void Cashbox::Serve(const Client& client) {
    IsBusy = true;
    auto start = std::chrono::steady_clock::now();
    FreeTimeInMilliseconds += (start - lastClientServed).count();

    for(int i = 0; i < client.Cart.size(); i++) {
        std::this_thread::sleep_for(productProcessDelay);
    }
    auto end = std::chrono::steady_clock::now();

    WorkTimeInMilliseconds += (end - start).count();
    lastClientServed = std::chrono::steady_clock::now();
    IsBusy = false;
}
