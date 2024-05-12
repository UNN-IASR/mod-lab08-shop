// Copyright 2024 alenapoliakova
#pragma once
#include <iostream>
#include <queue>
#include <random>
#include <thread>
#include <mutex>

int generateRandomNumber(int min, int max);
double calculate_r(double lyamda, double nu);
double calculate_P0(int pool_count, double r, int max_length);
double calculate_Pn(int pool_count, double r, double P0, int max_length);
double Factorial(double n);

struct Customer {
    int arrivalTime;
    int number_of_items;

};


struct Statics {
    int served_customers;
    int unserved_customers;
    double failureProbability;
    double relativeThroughput;
    double absoluteThroughput;
};


class Store {
private:
    int number_of_cash_registers;
    int customer_intensity;
    double processing_speed;
    int maximum_queue_length;
    std::queue <Customer> customer_queue;
    std::vector<int> checkout_times;
    std::mutex mtx;
    bool simulationFinished;
    int served_customers;
    int unserved_customers;
    double total_queue_length;
    double total_customer_stay_time;
    double total_checkout_time;
    double total_idle_time;
    double total_time;

public:
    Store(int registers, int intensity, int speed, int maxQueue);
    void simulate(int simulationTime, double average_items);
    Statics printStatistics();
    Statics printTeoreticStatics(double intensity, double speed, int number_of_cash_registers, int max_queue_length, double average_items);
    void cashierProcess();
    void customerProcess(double average_items);
    int getCurrentTime();
};
