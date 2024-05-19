#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>
#include <iostream>

struct Client {
public:
    int Id;
    int Cart;
    std::chrono::time_point<std::chrono::steady_clock> joinedQueue;
};

class Cash {
private:
    std::chrono::duration<int, std::chrono::milliseconds::period>
        productProcessDelay;
    std::chrono::time_point<std::chrono::steady_clock> lastClientServed;
public:
    bool IsBusy;
    int ClientsServed;
    double WorkTimeInMilliseconds;
    double FreeTimeInMilliseconds;
    Cash(int processingSpeedProducts = 10);
    void Serve(const Client& client);
};

class Shop {
private:
    int Products;
    bool isLoggingEnabled;
    std::mutex locker;
    std::mutex queueLocker;
    int maxQueue;
    std::queue<Client> clientsQueue;
    std::vector<Cash> cash;
    bool isWorking;
    std::chrono::duration<double, std::chrono::milliseconds::period>
        producerDelay;
    double avgProductsInCart;
    void Produce(int clientsCount);
    void Consume();
    void ProcessClient(const Client& client, Cash& cashbox);
    Client GenerateClient();
public:
    Shop(int maxQueue,
        double clientsIntensity,
        int numberCash,
        int processingSpeedProducts,
        double avgProductsInCart);
    int ServedClients = 0;
    int RejectedClients = 0;
    double AvgQueueLength = 0;
    double AvgClientTime = 0;
    double AvgCashboxWorkTime = 0;
    double AvgCashboxFreeTime = 0;
    void Simulate(int clientsCount);
    void EnableLogging();
    void DisableLogging();
};