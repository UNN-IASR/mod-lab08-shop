#pragma once
#include <queue>
#include <chrono>
#include <thread>
#include <mutex>
#include <iostream>
#include <cmath>
#include <string>

class Client {
private:
    int count_product;
    bool isServiced;
    std::chrono::system_clock::time_point start_time;
    std::chrono::system_clock::time_point end_time;
public:
    Client(int count_product);
    Client(const Client& client);

    void StartWait();
    void EndWait();
    std::chrono::milliseconds GetServiceTime();

    int GetCountProduct();
};

class Register {
private:
    int countClient;
    std::chrono::milliseconds work_time;

public:
    Register();
    Register(const Register& registers);
    void RunService(Client client, int speed_service);
    int GetCountClient();
    std::chrono::milliseconds GetWorkTime();
};

class Statistics {
private:
    int Factorial(int n);
    int n;
    int m;

public:
    const int count_serviced;
    const int count_failures;
    const int count_visitors;
    const int avg_len_client_queue;
    const std::chrono::milliseconds avg_time_serviced;
    const double lambda;
    const double mu;
    const int count_registers;
    const int maxLen_queue;
    const std::chrono::milliseconds time_work;
    const std::chrono::milliseconds avg_serviced_time_work;
    const std::chrono::milliseconds avg_serviced_downtime;

    double P0_theor();
    double Prej_theor();
    double Q_theor();
    double A_theor();

    double Prej_real();
    double Q_real();
    double A_real();

    Statistics(int count_services,
        int count_failures,
        int count_visitors,
        int avg_len_client_queue,
        std::chrono::milliseconds avg_time_serviced,
        double intensity,
        int speed_service,
        int count_registers,
        int maxLen_queue,
        std::chrono::milliseconds time_work,
        std::chrono::milliseconds avg_register_time_work,
        std::chrono::milliseconds avg_register_downtime);

    std::string ToString();
};

class Shop {
private:
    int count_registers;
    double client_intensity;
    int speed_service;
    int count_product;
    int maxLen_client_queue;

    std::queue<Client> client_queue;
    std::queue<Register> register_queue;
    std::mutex mtx_for_register_queue;
    std::mutex mtx_for_client_queue;

    std::mutex mtx_for_time;

    bool isEnd;
    int count_tact;

    int count_serviced;
    int count_failures;
    int count_visitors;
    int avg_len_client_queue;
    std::chrono::milliseconds avg_time_serviced_client;
    std::chrono::milliseconds time_work;
    std::chrono::milliseconds avg_register_time_work;
    std::chrono::milliseconds avg_register_downtime;

    void RunRegister(Client client, Register registers);

    Client SpawnClient();
    void AddClient();
    void Listen();

public:
    Shop(int count_registers, double intensity, int speed_service, int count_product, int maxLen_queue);
    void Run(std::chrono::milliseconds time_run);
    Statistics GetStatistics();
};