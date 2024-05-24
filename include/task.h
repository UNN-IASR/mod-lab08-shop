#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <future>
#include <cmath>
#include <queue>
#include <deque>
#include <vector>
#include <chrono>

class Customer {
public:
    int count_products;
    int time_waited;
    int time_served;
    std::chrono::system_clock::time_point time_start_in_queue;
};

class Cash {
public:
    bool busy;
    int number_cassa;
    double averge_opening_time_cassa;
    double averge_downtime_cassa;
    std::chrono::system_clock::time_point timeDownStart;
    
};
class Shop {
public: 
    double count_cassa;
    double intensity_customers;
    double mu;
    double average_number_of_products;
    double max_length_queue;

    int serviced_customers = 0;
    int unservesed_customers = 0;
    double average_length_queue = 0;
    double average_time_customer_in_queue = 0;
    double average_time_customer_at_cassa = 0;
    double average_time_work_cassa;
    double average_time_busy_cassa;

    std::mutex mutex_;
    std::queue<Customer> queue;
    std::vector<Cash> cassa;
    std::vector<std::thread*> number_threads;

    Shop(double numberRegisters, double intensity,
        double speed, double avgProducts, double maxQueue);
    double Factorial(int n);
    void Theoretical_result();
    void Work_cassa(int id, Customer client, double speed);
    void Processing();
    void Data_statistics();

    std::chrono::system_clock::time_point allTimeStart;
    int allTime = 0;

   

    //практические данные
    double Pre;//вероятность отказа
    double Q;//относительную пропускную способность магазина
    double A;//абсолютную пропускную способность

};
