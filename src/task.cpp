#include "../include/task.h"

Market::Market(int cCount, int Speed, int cRate, int cCart, int qLen) {
    chashboxesCount = cCount;
    timeToProcessProduct = Speed;
    ClientRate = cRate;
    ClientCart = cCart;
    maxQueueLength = qLen;
}


Client Market::CreateClient() {
    std::random_device dev;
    std::mt19937 rand(dev());
    std::uniform_int_distribution dist(1, 2*ClientCart-1);
    int cart = dist(rand);
    tp start = std::chrono::system_clock::now();
    return { cart, start };
};

void Market::Run() {
    while (work || Clients.size() != 0) {
        int time = 0;
        { std::lock_guard lock(mutex);
            if (Clients.size() != 0) {
                Client cust = Clients.front();
                Clients.pop();
                time = cust.cart * timeToProcessProduct;
                AvgClientTime += (std::chrono::system_clock::now() - cust.start).count() / 1000 + time;
                AvgCashboxWorkTime += time;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void Market::RunAll() {
    std::vector<std::thread> threads(chashboxesCount);
    for (int c = 0; c < chashboxesCount; c++)
        threads[c] = std::thread(&Market::Run, this);
    for (int c = 0; c < chashboxesCount; c++)
        threads[c].join();
}


void Market::Work(int clientNum) {
    std::random_device dev;
    std::mt19937 rand(dev());
    std::uniform_int_distribution dist(1, 2*ClientRate-1);
    std::thread serv(&Market::RunAll, this);
    for (int i = 0; i < clientNum; i++) {
        Client cust = CreateClient();
        mutex.lock();
        AvgQueueLength += Clients.size();
        if (Clients.size() < maxQueueLength) {
            Clients.push(cust);
            acceptedCustomer++;
        }
        else {
            rejectedCustomer++;
        }
        mutex.unlock();
        int time = 1000.0 / dist(rand);
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    work = false;
    serv.join();
}