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
    int sumLengths;
    int countLenghts;
    std::vector <int> lifeTimes;
    std::vector <std::chrono::time_point<std::chrono::system_clock>> lastTimePoint;
    std::vector <int> workTime;
    std::vector <int> waitTime;
    Stats();
    void openCash(int);
    void updateWorkTime(int);
    void updateWaitTime(int);
    void calcResults();
};

class Shopper{
public:
    std::chrono::time_point<std::chrono::system_clock> birth;
    int products;
    Shopper(int);
    int lifeTime();
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
    std::deque <Shopper> queue;
    void run();
    void incomingCustomers();
    void productProcessing(Shopper, int);
}; 
