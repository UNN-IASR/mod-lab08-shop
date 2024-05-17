// Copyright 2024 Mikhas3009

#ifndef INCLUDE_TASK_H_
#define INCLUDE_TASK_H_
#endif  

#include <vector>
#include <string>
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <cmath>
#include <iostream>

struct Product {
    public:
        Product(std::string name, double price);
        std::string Name;
        double Price;
};

struct Client {
    public:
        int Id;
        std::vector<Product> Cart;
        std::chrono::time_point<std::chrono::steady_clock> JoinedQueue;
};

class Cashbox {
    private:
        std::chrono::duration<int, std::chrono::milliseconds::period> 
            productProcessDelay;
        std::chrono::time_point<std::chrono::steady_clock> lastClientServed;
    public:
        bool IsBusy;
        int ClientsServed;
        double WorkTimeInMilliseconds;
        double FreeTimeInMilliseconds;
        Cashbox(int timeToProcessProduct = 10);
        void Serve(const Client& client);
};

class Market {
    private:
        static Product Products[];
        bool isLoggingEnabled;
        std::mutex locker;
        std::mutex queueLocker;
        int maxQueueLength;
        std::queue<Client> clientsQueue;
        std::vector<Cashbox> cashboxes;
        bool isWorking;
        std::chrono::duration<double, std::chrono::milliseconds::period>
            producerDelay;
        double avgProductsInCart;

        void Produce(int clientsCount);
        void Consume();
        void ProcessClient(const Client& client, Cashbox& cashbox);

        Client GenerateClient();
    public:
        Market(int maxQueueLength,
               double clientsIntensity,
               int cashboxesCount,
               int timeToProcessProduct,
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

#endif // INCLUDE_MARKET_SIMULATOR_H
