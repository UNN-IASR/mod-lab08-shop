#include "task.h"

ClientGenerator::ClientGenerator(Store* _store, int _cart_size) {
    store = _store;
    mt = std::mt19937(device());
    poiss = std::poisson_distribution<int>(_cart_size);
}

void ClientGenerator::Send(int id) {
    Client c;
    c.id = id;
    c.cart = poiss(mt);
    //std::cout << "Client " << c.id << " with " << c.cart << " going to shop\n";
    store->Receive(c);
}

Store::Store(int cash_num, double cash_speed, int queue_max) {
    cashNum = cash_num;
    cashSpeed = cash_speed;
    queueMax = queue_max;
    needWork = true;
    for (int i = 0; i < cash_num; i++)
        cashs.push_back(std::thread(&Store::CashWork, this));
}

void Store::Receive(Client client) {
    client.start = std::chrono::system_clock::now();
    std::unique_lock lock(mut);
    total++;
    totalQueue += queue.size();
    if (queue.size() < queueMax) {
        queue.push(client);
        //std::cout << "Client " << client.id << " in queue\n";
    } else rejected++;
}

void Store::CashWork() {
    while (needWork || !queue.empty()) {
        mut.lock();
        if (queue.empty()) {
            mut.unlock();
            continue;
        }
        Client cur = queue.front();
        queue.pop();
        //std::cout << "Client " << cur.id << " in progress\n";
        mut.unlock();
        cur.inCash = std::chrono::system_clock::now();
        auto delay = std::chrono::milliseconds((int)(1000.0 / cashSpeed * cur.cart));
        std::this_thread::sleep_for(delay);
        cur.end = std::chrono::system_clock::now();
        mut.lock();
        //std::cout << "Client " << cur.id << " done :)\n";
        accepteds.push_back(cur);
        mut.unlock();
    }
}

void Store::Close() {
    needWork = false;
    for (int i = 0; i < cashNum; i++)
        cashs[i].join();
}

void Store::Stats() {
    accepted = accepteds.size();
    avgQueue = (double)totalQueue / total;
    double temp = 0;
    for (int i = 0; i < accepteds.size(); i++) {
        auto diff = accepteds[i].inCash - accepteds[i].start;
        temp += std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    }
    avgInQueue = temp / accepted / 1000;
    temp = 0;
    for (int i = 0; i < accepteds.size(); i++) {
        auto diff = accepteds[i].end - accepteds[i].inCash;
        temp += std::chrono::duration_cast<std::chrono::milliseconds>(diff).count();
    }
    avgInCash = temp / accepted / 1000;
    avgCashWork = temp / cashNum / 1000;
}