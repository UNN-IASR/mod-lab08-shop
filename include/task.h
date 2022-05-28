#pragma once

#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <chrono>
#include <cmath>
#include <thread>

class Client {
 private:
    int timeQueue;
    int timeCashbox;
    int items;
    unsigned int id;

    static unsigned int gen_id;

    std::chrono::system_clock::time_point tin;

 public:
    Client(int avgItems);
    unsigned int GetId();
    int GetQueueTime();
    int GetCashboxTime();
    void EnterQueue();
    void ExitQueue();
    void ExitCashbox(int time);
    int GetItems();
};

class Shop {
 private:
    struct Cashbox {
        int waitStat;
        int workStat;
        std::thread thrd;
    };
    int cashbox;
    std::vector<Cashbox> cboxes;
    int servicespeed;
    int maxqueue;
    int requestRate;
    int avgItems;

    std::deque<Client *> queue;
    std::mutex mu;

    int completedCount;
    int rejectedCount;

    int queueLen;
    int countProc;

 public:
    Shop(int cashboxCount, int requestRate, double servicespeed, int avgItems, int maxCount);
    void Start(int index);
    std::string Modelling();
};
