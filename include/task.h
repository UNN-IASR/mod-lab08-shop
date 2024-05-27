#ifndef TASK_H
#define TASK_H

#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include <atomic>
#include <memory>

struct Customer {
    int id;
    int items;
    std::chrono::system_clock::time_point arrival_time;
};

class Cashier {
public:
    Cashier(int id, int processing_speed);
    void serveCustomer(Customer customer);
    void waitForCustomer();
    int getId() const;
    bool isBusy() const;
    long getTotalWorkTime() const;
    long getTotalIdleTime() const;

private:
    int id;
    int processing_speed;
    std::atomic<bool> busy;
    long total_work_time;
    long total_idle_time;
    std::chrono::system_clock::time_point last_end_time;
    std::mutex mx;
    std::condition_variable cv;
};

class Supermarket {
public:
    Supermarket(int num_cashiers, int customer_flow_rate, int processing_speed, int avg_items_per_cart, int max_queue_length);
    void runSimulation(int duration_s);
    void printStatistics() const;
    void printTheoreticalValues() const;

private:
    std::vector<std::unique_ptr<Cashier>> cashiers;
    std::queue<Customer> customer_queue;
    std::mutex queue_mx;
    std::condition_variable queue_cv;
    std::atomic<int> customers_served;
    std::atomic<int> customers_not_served;
    int customer_flow_rate;
    int avg_items_per_cart;
    int max_queue_length;
    std::atomic<long> total_queue_length;
    std::atomic<int> total_customers;
    bool simulation_finished;

    void generateCustomers(std::chrono::system_clock::time_point end_time);
    void operateCashier(Cashier* cashier);
};

#endif // TASK_H
