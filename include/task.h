#include <deque>
#include <vector>
#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include <random>
#include <mutex>

struct Stats {
    int served;
    int rejected;
    double avgQueueLength;
    double avgServeTime;
    double avgCashWorkTime;
    double avgCashWaitTime;
    Stats();
};

class Shop {
private:
    int cash;
    double shoppersIntensity;
    double productCashTime;
    int maxQueueLength;
    double avgProducts;
    int customers;
    int customersVisited;
    std::vector <bool> isBusy;
    std::vector <std::thread> cashiers;
public:
    Stats statistic;
    Shop(int, double, double, double, int, int);
    ~Shop();
    std::deque <int> queue;
    void run();
    void incomingCustomers();
    void productProcessing(int, int);
}; 
