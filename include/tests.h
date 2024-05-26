// Copyright 2021 GHA Test Team
#ifndef TASK_H
#define TASK_H

#include <queue>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include <atomic>

class Customer {
public:
    int id;
    int num_items;

    Customer(int id, int num_items);
};

class Checkout {
public:
    int id;
    std::atomic<bool> busy;
    std::atomic<int> total_served;
    std::atomic<int> total_idle_time;
    std::atomic<int> total_service_time;
    std::chrono::time_point<std::chrono::steady_clock> start_idle;

    Checkout(int id);
    void serveCustomer(Customer customer, int processing_speed);
};

class Supermarket {
public:
    int num_checkouts;
    int customer_arrival_rate;
    int processing_speed;
    int avg_cart_size;
    int max_queue_length;
    
    std::vector<std::unique_ptr<Checkout>> checkouts;
    std::queue<Customer> queue;
    std::mutex queue_mutex;
    std::condition_variable cv;
    std::atomic<int> customers_served;
    std::atomic<int> customers_not_served;
    std::atomic<int> total_queue_time;
    std::atomic<int> total_customers_in_queue;

    Supermarket(int num_checkouts, int customer_arrival_rate, int processing_speed, int avg_cart_size, int max_queue_length);

    void customerGenerator();
    void checkoutWorker(Checkout& checkout);
    void runSimulation();
    void printStatistics();
    double calculateTheoreticalP0();
    double calculateTheoreticalPrej(double P0);
    double calculateTheoreticalQ(double Prej);
    double calculateTheoreticalA(double Q);
};

#endif // TASK_H
