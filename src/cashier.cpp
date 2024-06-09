#include <queue>
#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>
#include "cashier.h"
using namespace std::chrono_literals;

cashier::cashier(int n, int _clientPool, int _productSpeed, int* _stop)
{
    name = "cashier" + std::to_string(n);
    waitTime = new double();
    workTime = new double();
    prouctSpeed = _productSpeed;
    clientPool = _clientPool;
    q = new std::queue<int>();
    c_lock = new std::mutex();
    stop = _stop;
    trd = std::thread(&cashier::Start, this,q, prouctSpeed, workTime,waitTime,stop,name);
}

void cashier::Start(std::queue<int>* q, int prouctSpeed, double* workTime, double* waitTime, int* stop,std::string name) {
    
    double _workTime = 0;
    double _waitTime = 0;

    while (true)
    {
        std::chrono::time_point<std::chrono::system_clock> start, end;
        start = std::chrono::system_clock::now();
        
        c_lock->lock();
        if (!q->empty()) {
            std::this_thread::sleep_for(q->front() * prouctSpeed * 1ms);
            std::cout << name << " " << " done client with " << q->front() << " products" << std::endl;
            q->pop();
            end = std::chrono::system_clock::now();
            //end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            _workTime += elapsed_seconds.count();
        } else {
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start;
            _waitTime += elapsed_seconds.count();
        }
        c_lock->unlock();

        if (*stop == 20 && q->empty()) break;
    }

    *workTime = _workTime;
    *waitTime = _waitTime;

    return;
}