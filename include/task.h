// Copyright 2022 UNN-IASR
#pragma once
#include <iostream>
#include <vector>
#include <random>
#include <queue>
#include <mutex>
#include <list>
#include <thread>
#include <condition_variable>
#include <functional>

typedef std::vector<std::thread*> workingCashboxes;

using namespace std;

class Customer {
 public:
    std::vector<int> check;
    explicit Customer(std::vector<int> check1);
};

class Supermarket {
 private:
     // for statistics
    int requests_flow;
    int served_customers; // обслуженные покупатели
    int unserved_customers; // необслуженные покупатели
    double average_line_len = 1.0; // средняя длина очереди
    double average_customer_time_line_and_cashbox = 0.0; // среднее время нахождение покупателя в очереди + на кассе
    double average_work_time = 0.0; // среднее время работы кассы
    double average_down_time = 0.0; // среднее время простоя кассы

    int all_lines = 0;
    int cashboxes_number; // количество касс
    int line_len; // длина очереди
    int max_num_of_customers;
    int customers_intensity; // интенсивность потока покупателей 
    int serving_speed; // скорость обработки товара на кассе
    double average_product_num = 0.0; // среднее количество товаров в тележке покупателя
    int max_line_len = 0; // максимальная длина очереди
    int working_cashboxes = 0;
    double cashbox_downtime = 0.0;
    double cashbox_worktime = 0.0;
    bool finished = false;
    int all_checks_for_customers = 0;
    workingCashboxes cashboxes_WIP;
    std::vector<std::queue<Customer*>*> lines;
    std::mutex myMutex;

 public:

    Customer* getCustomer();
    void start();
    void serveCustomer(Customer* customer, int number);
    void serveLine(std::queue<Customer*>* customers);
    void serveSupermarket();
    int workingCashboxesNumber();
    double getAverageLineLength();
    double getAverageWaitingTimeForCustomer();
    double getAverageWorkTime();
    double getAverageDownTime();
    int getRequestsFlow();
    int getAmountOfServedCustomers();
    int getAmountOfUnservedCustomers();
    explicit Supermarket(int cashboxes, int customers_intensity, int serving_speed, int average_product_num, int max_line_len, int max_num_of_customers);

};
