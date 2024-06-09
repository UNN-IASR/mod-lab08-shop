#include <thread>
#include <cstdlib>
#include <queue>
#include <chrono>
#include <random>
#include <vector>
#include <iostream>
#include "task.h"
#include "cashier.h"

task::task(
    int _channelsCount,
    int _clientIntensity,
    int _channelPool,
    int _avgProducts,
    int _productSpeed,
    int _clientsCount) {
    channelsCount = _channelsCount;
    clientIntensity = _clientIntensity;
    channelPool = _channelPool;
    productSpeed = _productSpeed;
    avgProducts = _avgProducts;
    clientsCount = _clientsCount;
    avgWorktime = 0;
    avgWaittime = 0;
    avgQsize = 0;
    currClients = 0;
    for (int i = 0; i < channelsCount; i++)
    {
        
        channels.push_back(cashier(i, channelPool, productSpeed,&currClients));
        Qsize.push_back(channels[i].q->size());
        channels[i].stop = &currClients;
    }
    
}

void task::Start() {

    using namespace std::chrono_literals;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> products(1, avgProducts * 2);

    for (; currClients < clientsCount;) {
        int skippedChannel = 0;
        std::this_thread::sleep_for(1000 / clientIntensity * 1ms);
        for (int i = 0; i < channelsCount; i++) {
            if (channels[i].q->size() < channelPool) {
                Qsize[i] = channels[i].q->size();
                channels[i].q->push(products(rng));
                std::cout << channels[i].name << " got client" << std::endl;
                currClients++;
                //channels[i].c_lock->unlock();
                sendedClients++;
            } else {
                Qsize[i] = channelPool;
                std::cout << channels[i].name << " full" << std::endl;
                skippedChannel++;
            }
        }
        if (skippedChannel == channelsCount) {
            std::cout << "lost client\n";
            skippedClients++;
            currClients++;
        }
    }

    std::this_thread::sleep_for(avgProducts * 1 * productSpeed * clientsCount * 1ms);

    for (int i = 0; i < channelsCount; i++) {
        channels[i].trd.join();
        avgWorktime += *channels[i].workTime;
        avgWaittime += *channels[i].waitTime;
    }

    avgWorktime /= channelsCount;
    avgWaittime /= channelsCount;

    int sum = 0;

    for (auto start{ Qsize.begin() }; start != Qsize.end(); start++)
    {
        sum += *start;
    }

    avgQsize = sum / Qsize.size();
}

