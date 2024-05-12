#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>
#include <queue>

using tp = std::chrono::system_clock::time_point;

struct Customer {
    int cart;
    tp start;
};

class Market {
private:
    std::queue<Customer> customers;
    int cashiersNum;
    int cashiersSpeed;
    int customerRate;
    int customerCart;
    int queueSize;
    Customer CreateCustomer();
    void ServeAll();
    void ServeOne();
    std::mutex mutex;
    bool work;
public:
    Market(int, int, int, int, int);
    void Work(int);
    double statAccepted = 0;
    double statRejected = 0;
    double statQueueLen = 0;
    double statCustTime = 0;
    double statCashTime = 0;
};