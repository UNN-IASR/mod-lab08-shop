#include "task.h"

Market::Market(int cNum, int cSpeed, int cRate, int cCart, int qSize) {
    cashiersNum = cNum;
    cashiersSpeed = cSpeed;
    customerRate = cRate;
    customerCart = cCart;
    queueSize = qSize;
}

void Market::Work(int clientNum) {
    std::random_device dev;
    std::mt19937 rand(dev());
    std::uniform_int_distribution dist(1, 2*customerRate-1);
    std::thread serv(&Market::ServeAll, this);
    for (int i = 0; i < clientNum; i++) {
        Customer cust = CreateCustomer();
        mutex.lock();
        statQueueLen += customers.size();
        if (customers.size() < queueSize) {
            customers.push(cust);
            statAccepted++;
        }
        else {
            statRejected++;
        }
        mutex.unlock();
        int time = 1000.0 / dist(rand);
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
    work = false;
    serv.join();
}

void Market::ServeOne() {
    while (work || customers.size() != 0) {
        int time = 0;
        { std::lock_guard lock(mutex);
            if (customers.size() != 0) {
                Customer cust = customers.front();
                customers.pop();
                time = cust.cart * cashiersSpeed;
                statCustTime += (std::chrono::system_clock::now() - cust.start).count() / 1000 + time;
                statCashTime += time;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(time));
    }
}

void Market::ServeAll() {
    std::vector<std::thread> threads(cashiersNum);
    for (int c = 0; c < cashiersNum; c++)
        threads[c] = std::thread(&Market::ServeOne, this);
    for (int c = 0; c < cashiersNum; c++)
        threads[c].join();
}


Customer Market::CreateCustomer() {
    std::random_device dev;
    std::mt19937 rand(dev());
    std::uniform_int_distribution dist(1, 2*customerCart-1);
    int cart = dist(rand);
    tp start = std::chrono::system_clock::now();
    return { cart, start };
};