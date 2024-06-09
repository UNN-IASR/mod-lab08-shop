#pragma once
#include <iostream>
#include <thread>
#include "cashier.h"
#include <mutex>

class task
{
private:
    std::vector<cashier> channels;
    int channelsCount;
    int clientIntensity;
    int channelPool;
    int productSpeed;
    int avgProducts;
    int clientsCount;
    int currClients;
    std::vector<int> Qsize;
    std::thread trd;
public:
    int skippedClients;
    int sendedClients;
    double avgQsize;
    double avgWorktime;
    double avgWaittime;
    task(
        int channelsCount,
        int clientIntensity,
        int channelPool,
        int avgProducts,
        int productSpeed,
        int clientsCount);
    void Start();
};