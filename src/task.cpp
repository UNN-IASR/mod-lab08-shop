// Copyright 2022 UNN-IASR
#include "task.h"
#include <iostream>

double F(double n) {
    if (n == 0)
        return 1;
    else
        return n * F(n - 1);
}

Client::Client(int avgItems, int i) {
    tQueue = 0;
    tService = 0;
    Products = rand() % (2 * avgItems) + 1;
    id = i;
}

void Client::Qin() {
    timestart = std::chrono::system_clock::now();
}

void Client::Qout() {
    tQueue = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timestart).count();
}

void Client::outCashbox(int time) {
    tService = time;
}


Shop::Shop(int countcashbox, int requestrate, int servicespeed, int avgitems, int maxqueue) {

    money = countcashbox; 
    requestRate = requestrate; 
    speed = servicespeed; 
    avgItems = avgitems;
    maxQueue = maxqueue; 


    servedClients = 0;
    rejectedClients = 0;

    cashboxes = std::vector<Money>();
    queueClients = std::deque<Client*>();

    for (int i = 0; i < money; i++) {
        cashboxes.push_back(Money());
        cashboxes[i].tWait = 0;
        cashboxes[i].tWork = 0;
        cashboxes[i].thrd = std::thread(&Shop::Start, this, i);
    }
}

void Shop::Start(int index) {
    while (servedClients + rejectedClients < requestRate) {
        m.lock();

        if (queueClients.size() > 0) {
            queueClients.at(0)->Qout();

            int basket = queueClients.at(0)->Products;
            int id = queueClients.at(0)->id;

            cashboxes[index].tWork = cashboxes[index].tWork + (basket * speed);

            queueClients.at(0)->outCashbox(basket * speed);
            queueClients.pop_front();

            m.unlock();
            while (basket > 0) {
                
                std::this_thread::sleep_for(std::chrono::milliseconds(speed));
                basket -= 1;
            }
            servedClients++;
        } else {
            cashboxes[index].tWait = cashboxes[index].tWait + 10;
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void Shop::Work() {
    srand(time(0));

    int countClients = 0;
    std::vector<Client> clients;

    for (int i = 0; i < requestRate; i++) {
        clients.push_back(Client(avgItems, i + 1));
    }

    double avgQueueLen = 0.0;

    while (countClients < requestRate) {
        int time = rand() % 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));

        m.lock();
        avgQueueLen = avgQueueLen + queueClients.size();

        if (queueClients.size() < maxQueue)
        {
            queueClients.push_back(&clients.at(countClients));
            clients.at(countClients).Qin();
            m.unlock();
        } else {
            rejectedClients++;
            std::cout << "Klient nomer " << clients.at(countClients).id << " ne obsluzhen" << std::endl;

            m.unlock();
        }
        countClients++;
    }

    double aWait = 0.0;
    double aWork = 0.0;
    double aTimeinqueue = 0.0;
    double aTimeincashbox = 0.0;
    double workTime = 0.0;

    for (int i = 0; i < money; i++) {
        cashboxes[i].thrd.join();
        aWait = aWait + cashboxes[i].tWait;
        aWork = aWork + cashboxes[i].tWork;
    }

    for (int i = 0; i < requestRate; i++) {
        aTimeinqueue = aTimeinqueue + clients.at(i).tQueue;
        aTimeincashbox = aTimeincashbox + clients.at(i).tService;
    }

    avgQueueLen = avgQueueLen / (double)countClients;
    workTime = aWork + aWait;
    aWait = aWait / (double)money;
    aWork = aWork / (double)money;
    aTimeinqueue = aTimeinqueue / (double)servedClients;
    aTimeincashbox = aTimeincashbox / (double)servedClients;

    std::cout << "Served clients: " << std::to_string(servedClients) << std::endl;
    std::cout << "Rejected clients: " << std::to_string(rejectedClients) << std::endl;
    std::cout << "Average queue: " << std::to_string(avgQueueLen) << std::endl;
    std::cout << "Avegage client time: " << std::to_string(aTimeinqueue / 1000) << " i " << std::to_string(aTimeincashbox / 1000) << std::endl;
    std::cout << "Average cashbox work time: " << std::to_string(aWork / 1000) << std::endl;
    std::cout << "Average cashbox stop time: " << std::to_string(aWait / 1000) << std::endl;

    double la = 4.0;
    double mu = 2.0;
    double p = la / mu;

    double P0 = 1.0;
    for (int i = 1; i <= money; i++)
        P0 = P0 + pow(p, i) / F(i);

    for (int i = money + 1; i < money + maxQueue; i++)
        P0 = P0 + pow(p, i) / (F(money) * pow(money, i - money));

    P0 = 1.0 / P0;
    double P = P0 * pow(p, (money + maxQueue)) / (double)(pow(money, maxQueue) * F(money));
    double Q = 1.0 - P;
    double A = la * Q;
    std::cout << "p = " << std::to_string(p) << std::endl;
    std::cout << "P0 = " << std::to_string(P0) << std::endl;
   

    std::cout << "Refusual: " << std::to_string(P) << std::endl;
    std::cout << " Relative Throughput: " << std::to_string(Q) << std::endl;
    std::cout << "Absolute Throughput: " << std::to_string(A) << std::endl;
}
