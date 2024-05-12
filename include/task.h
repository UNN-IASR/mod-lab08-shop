#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <cmath>
#include <random>

using time_point = std::chrono::system_clock::time_point;

struct Client {
    int id;
    int cart;
    time_point start;
    time_point inCash;
    time_point end;
};

class Store {
    private:
    std::vector<std::thread> cashs;
    std::queue<Client> queue;
    std::vector<Client> accepteds;
    int cashNum;
    double cashSpeed;
    int queueMax;
    bool needWork;
    std::mutex mut;
    int totalQueue = 0;
    public:
    Store(int cash_num, double cash_speed, int queue_max);
    void Receive(Client client);
    void CashWork();
    void Close();
    int accepted;
    int rejected = 0;
    int total = 0;
    double avgQueue;
    double avgInQueue;
    double avgInCash;
    double avgCashWork;
    void Stats();
};


class ClientGenerator {
    private:
    Store* store;
    std::random_device device;
    std::mt19937 mt;
    std::poisson_distribution<int> poiss;
    public:
    ClientGenerator(Store* _store, int _cart_size);
    void Send(int id);
};
