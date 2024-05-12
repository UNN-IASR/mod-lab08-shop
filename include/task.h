#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <mutex>
#include <chrono>
#include <queue>

using tp = std::chrono::system_clock::time_point;

struct Client {
    int cart;
    tp start;
};

class Market {
private:
    std::queue<Client> Clients;
    int chashboxesCount;
    int timeToProcessProduct;
    int ClientRate;
    int ClientCart;
    int maxQueueLength;
    Client CreateClient();
    void RunAll();
    void Run();
    std::mutex mutex;
    bool work;
public:
    Market(int, int, int, int, int);
    void Work(int);
    double acceptedCustomer = 0;
    double rejectedCustomer = 0;
    double AvgQueueLength = 0;
    double AvgClientTime = 0;
    double AvgCashboxWorkTime = 0;
};