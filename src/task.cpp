#include "../include/task.h"

Shop::Shop(int maxQueue,
    double clientsIntensity,
    int numberCash,
    int processingSpeedProducts,
    double avgProductsInCart) {
    this->maxQueue = maxQueue;
    for (int i = 0; i < numberCash; i++) {
        cash.push_back(Cash(processingSpeedProducts));
    }
    this->avgProductsInCart = avgProductsInCart;
    producerDelay = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double>(1 / clientsIntensity)
    );
    isLoggingEnabled = false;
}

void Shop::Simulate(int clientsCount) {
    isWorking = true;
    std::thread producerThread(&Shop::Produce, this, clientsCount);
    std::thread consumerThread(&Shop::Consume, this);
    producerThread.join();
    isWorking = false;
    consumerThread.join();
    AvgQueueLength /= clientsCount;
    AvgClientTime /= clientsCount;
    for (int i = 0; i < cash.size(); i++) {
        AvgCashboxFreeTime += cash[i].FreeTimeInMilliseconds;
        AvgCashboxWorkTime += cash[i].WorkTimeInMilliseconds;
    }
    AvgCashboxFreeTime /= cash.size();
    AvgCashboxWorkTime /= cash.size();
}

void Shop::Produce(int clientsCount) {
    for (int i = 0; i < clientsCount; i++) {
        std::this_thread::sleep_for(producerDelay);
        AvgQueueLength += clientsQueue.size();
        auto client = GenerateClient();
        client.Id = i;
        if (isLoggingEnabled) {
            std::cout << "Ñãåíåðèðîâàí êëèåíò " << client.Id
                << "(Ïðîäóêòîâ: " << client.Cart << ")"
                << std::endl;
        }
        queueLocker.lock();
        if (clientsQueue.size() == maxQueue) {
            RejectedClients++;
            queueLocker.unlock();
            continue;
        }
        client.joinedQueue = std::chrono::steady_clock::now();
        clientsQueue.push(client);
        queueLocker.unlock();
    }
}

Client Shop::GenerateClient() {
    int minProducts = 1;
    int maxProducts = std::ceil(avgProductsInCart * 2 - minProducts);
    int productsInCart = rand() % maxProducts + minProducts;
    Client client;
    client.Cart = productsInCart;
    return client;
}

void Shop::Consume() {
    std::vector<std::thread> threadPool(cash.size());
    while (isWorking || !clientsQueue.empty()) {
        for (int i = 0; i < cash.size(); i++) {
            if (cash[i].IsBusy) {
                continue;
            }
            if (threadPool[i].joinable()) {
                threadPool[i].join();
            }
            queueLocker.lock();
            if (!clientsQueue.empty()) {
                auto client = clientsQueue.front();
                clientsQueue.pop();
                threadPool[i] = std::thread(&Shop::ProcessClient,
                    this,
                    std::ref(client),
                    std::ref(cash[i]));
                if (isLoggingEnabled) {
                    std::cout << "Îáñëóæèâàíèå êëèåíòà "
                        << client.Id << "(Ïîòîê: "
                        << threadPool[i].get_id() << ")" << std::endl;
                }
            }
            queueLocker.unlock();
        }
    }
    for (int i = 0; i < threadPool.size(); i++) {
        if (threadPool[i].joinable())
            threadPool[i].join();
    }
}

void Shop::ProcessClient(const Client& client, Cash& cashbox) {
    cashbox.Serve(client);
    locker.lock();
    ServedClients++;
    auto point = std::chrono::steady_clock::now();
    auto duration = point - client.joinedQueue;
    AvgClientTime += duration.count();
    locker.unlock();
}

void Shop::EnableLogging() {
    isLoggingEnabled = true;
}

void Shop::DisableLogging() {
    isLoggingEnabled = false;
}

Cash::Cash(int timeToProcessProduct) {
    productProcessDelay = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::duration<double>((double)timeToProcessProduct / 1000));
    lastClientServed = std::chrono::steady_clock::now();
    IsBusy = false;
}

void Cash::Serve(const Client& client) {
    IsBusy = true;
    auto start = std::chrono::steady_clock::now();
    FreeTimeInMilliseconds += (start - lastClientServed).count();
    for (int i = 0; i < client.Cart; i++) {
        std::this_thread::sleep_for(productProcessDelay);
    }
    auto end = std::chrono::steady_clock::now();
    WorkTimeInMilliseconds += (end - start).count();
    lastClientServed = std::chrono::steady_clock::now();
    IsBusy = false;
}
