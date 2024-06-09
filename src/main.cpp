#include <gtest/gtest.h>
#include "task.h"
#include "cashier.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

int main() {
    int channelsCount = 4;
    double clientIntensity = 10;
    int handlingSpeed = 5;
    int channelPool = 5;
    int avgProducts = 5;
    int productSpeed = 100;
    int clientsCount = 20;

    task t(channelsCount, clientIntensity, channelPool,avgProducts,productSpeed,clientsCount);
    t.Start();

    std::cout << "avgQsize " << t.avgQsize << std::endl;
    std::cout << "gotedClients " << t.sendedClients << std::endl;
    std::cout << "lostClients " << t.skippedClients << std::endl;
    std::cout << "avgWorktime: " << t.avgWorktime << std::endl;
    std::cout << "avgWaittime: " << t.avgWaittime << std::endl;
}