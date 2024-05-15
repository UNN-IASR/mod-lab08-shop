#pragma once

#include <iostream>
#include <random>
#include <queue>
#include <vector>
#include <thread>
#include <memory>
#include <mutex>

class Shop {
private:
    int count;
    int intensity;
    int speed;
    int av_product;
    int max_length;
    int max_customer = 30;
    int served = 0;
    double time = 0;
    double av_lenght = 0;
    double av_time = 0;
    double av_work_time = 0;
    double av_wait_time = 0;
    struct Customer {
        std::vector<int> shops;
        Customer(const std::vector<int>& check) : shops(check) {}
    };
    using queueCust = std::shared_ptr<std::queue<std::shared_ptr<Customer>>>;
    std::shared_ptr<Customer> Gen_customer();
    std::vector<std::thread> threads;
    std::vector<queueCust> queue_B;
    std::mutex mutex;
public:
    Shop(int count, int intensity, int speed,
        int av_product, int max_length);
    void Run();
    void Serv_customers(Customer& cust);
    void Serv_Q(queueCust queue);
    void Statistic();
    int Served();
    int Unserved();
    double Av_len();
    double Av_time();
    double Av_work_time();
    double Av_wait_time();
    double Failure_probability();
    double Relative_throughput();
    double Absolute_throughput();
};
