#pragma once
#include <thread>
#include <queue>
#include <chrono>
#include <string>
#include <mutex>

class cashier
{
private:
    int clientPool;
    int prouctSpeed;
public:
    int* stop;
    double* workTime;
    double* waitTime;
    std::string name;
    void Start(std::queue<int>*,int,double*,double*,int*,std::string);
    std::queue<int>* q;
    std::thread trd;
    std::mutex* c_lock;
    cashier(int, int, int, int*);
};