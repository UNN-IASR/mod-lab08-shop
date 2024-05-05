// Copyright 2024 alenkaLo
#include "../include/task.h"

#include <iostream>

Shop::Shop(double _countCash, double _li, double _mu, double _countProduct, double _maxQueue) {
    countCash = _countCash;
    li = _li;
    mu = _mu;
    countProduct = _countProduct;
    maxlenQueue = _maxQueue;
    allTime = 0;
    servedСustomers = 0;
    unservedCustomers = 0;
    lenQueue = 0;
    timeWaitedClient = 0;
    timeServedClient = 0;

    queue = std::queue<Client>();
    cash = std::vector<Cash>();
    ts = std::vector<std::thread*>();

    for (int i = 0; i < countCash; i++) {
        cash.push_back(Cash());
        cash[i].index = i;
        cash[i].workTime = 0;
        cash[i].waitTime = 0;
        cash[i].free = true;
        cash[i].timeDownStart = std::chrono::system_clock::now();
    }
}

void Shop::WorkCash(int id, Client client, double speed) {
    std::unique_lock<std::mutex> mu(myMutex);

    cash[id].waitTime += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - cash[id].timeDownStart).count();
    cash[id].workTime += client.products / speed;
    client.timeServed = (int)(client.products / speed);

    std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.products / speed)));

    cash[id].timeDownStart = std::chrono::system_clock::now();
    cash[id].free = true;

    mu.unlock();
}

void Shop::Work() {
    allTimeStart = std::chrono::system_clock::now();

    srand(time(0));
    int time = 0;
    int nextCustomerTime = rand() % 10 + 1;
    int numberClients = 0;

    while (true) {
        if (numberClients < li) {
            time++;
            if (time == nextCustomerTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                numberClients++;
                Client client;
                client.products = rand() % (2 * (int)countProduct) + 1;
                client.timeQueueStart = std::chrono::system_clock::now();

                if (queue.size() < maxlenQueue) {
                    queue.push(client);
                }
                else {
                    unservedCustomers++;
                }
                nextCustomerTime = time + rand() % 10 + 1;
            }
        }

        for (int i = 0; i < countCash; i++) {
            if ((queue.size() > 0) && (cash[i].free == true)) {
                cash[i].free = false;

                queue.front().timeWaited = std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now() - queue.front().timeQueueStart).count();

                ts.push_back(new std::thread(&Shop::WorkCash, this, i, queue.front(), mu));

                //cобираем статистику
                lenQueue += queue.size();
                timeWaitedClient += queue.front().timeWaited;
                timeServedClient += queue.front().products / mu;

                servedСustomers++;
                queue.pop();
            }
        }
        if (queue.empty() && numberClients == li) break;
    }
    for (auto& th : ts)
        th->join();

    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();
}

double factorial(double n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

void Shop::Statistics() {
    timeWorkedCash = 0;
    timeWaitedCash = 0;

    for (int i = 0; i < countCash; i++) {
        timeWorkedCash += cash[i].workTime;
        timeWaitedCash += cash[i].waitTime;
    }

    lenQueue = lenQueue / li;
    timeWaitedClient = timeWaitedClient / li;
    timeServedClient = timeServedClient / li;

    timeWorkedCash = timeWorkedCash / countCash;
    timeWaitedCash = timeWaitedCash / countCash;

    double la = li / allTime;
    double mu = (double)servedСustomers / allTime;
    double p = la / mu;

    //практические
    double P = unservedCustomers / li;
    double Q = 1.0 - P;
    double A = la * Q;

    pPn = P;
    pQ = Q;
    pA = A;

    //теоретические
    double P0 = 1.0;
    for (int i = 1; i <= countCash; i++)
        P0 = P0 + pow(p, i) / factorial(i);

    for (int i = countCash + 1; i < countCash + maxlenQueue; i++)
        P0 = P0 + pow(p, i) / (factorial(countCash) * pow(countCash, i - countCash));

    P0 = 1.0 / P0;

    P = P0 * pow(p, (countCash + maxlenQueue)) / (double)(pow(countCash, maxlenQueue) * factorial(countCash));
    Q = 1.0 - P;
    A = la * Q;

    tPn = P;
    tQ = Q;
    tA = A;
}

