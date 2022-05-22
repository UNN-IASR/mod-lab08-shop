#pragma once
#include <vector>
#include <thread>
#include <string>
#include <iostream>
#include <queue>
#include <chrono>
#include <mutex>

class shop;
class customer
{
public:
    customer(unsigned int id, unsigned int cart_size);
    void inqueued();
    std::chrono::milliseconds serviced();
    unsigned int get_id();
    unsigned int get_cart();
private:
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    unsigned int id;
    unsigned int cart_size;
};

class cashbox
{
    friend class shop;
public:
    cashbox(shop* host, unsigned int id, std::chrono::milliseconds item_time);
    void operator()();
private:

    std::chrono::milliseconds work_time{ 0 };
    std::chrono::milliseconds wait_time{ 0 };
    std::chrono::time_point<std::chrono::system_clock> work_start;
    std::chrono::time_point<std::chrono::system_clock> work_end;
    unsigned int customers_serviced = 0;
    unsigned int id;
    shop* host;
    std::chrono::milliseconds item_time;
};

class shop
{
    friend class cashbox;
public:
    shop(unsigned int queue_length, unsigned int cashboxes_count, std::chrono::milliseconds item_time);
    void service(customer client);
    void close();
    void print_stat(double input_intencity);
    unsigned int get_queue_size();
    ~shop();
private:
    std::mutex mut;
    bool shop_available;
    unsigned int queue_max_length;

    std::chrono::milliseconds mean_work_time{ 0 };
    std::chrono::milliseconds mean_wait_time{ 0 };
    long long customers_serviced = 0;
    long long customers_left = 0;
    long long mean_queue_length = 0;
    long long serv_time = 0;

    std::vector<std::thread> workers;
    std::vector<cashbox*> cashboxes;
    std::queue<customer> clients;
};
