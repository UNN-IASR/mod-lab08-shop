// Copyright 2024 alenapoliakova
#include "task.h"

int generateRandomNumber(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
};


Store::Store(int registers, int intensity, int speed, int maxQueue) {
    number_of_cash_registers = registers;
    customer_intensity = intensity;
    processing_speed = speed;
    maximum_queue_length = maxQueue;
    served_customers = 0;
    unserved_customers = 0;
    total_queue_length = 0;
    total_customer_stay_time = 0;
    total_checkout_time = 0;
    total_idle_time = 0;
    simulationFinished = false;
    checkout_times.resize(registers, 0);
    total_time = 0;

};


void Store::simulate(int simulationTime, double average_items) {
    auto start = std::chrono::high_resolution_clock::now();
    simulationFinished = false;
    std::vector<std::thread> cashierThreads;
    for (int i = 0; i < number_of_cash_registers; i++) {
        cashierThreads.emplace_back(&Store::cashierProcess, this);
    }
    std::thread customerThread(&Store::customerProcess, this, average_items);
    auto seconds = std::chrono::duration<double>(simulationTime);
    std::this_thread::sleep_for(seconds);
    simulationFinished = true;

    auto end = std::chrono::high_resolution_clock::now();
    total_time = (double)(end - start).count() / 1000000000;
    total_idle_time = total_time - total_checkout_time / number_of_cash_registers;
    for (auto& thread : cashierThreads) {
        thread.join();
    }
    customerThread.join();

};


void Store::cashierProcess() {
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        auto current = std::chrono::high_resolution_clock::now();

        if (!customer_queue.empty()) {
            Customer customer = customer_queue.front();
            customer_queue.pop();

            auto operating = (double)customer.number_of_items / processing_speed;
            auto checkoutTime = std::chrono::duration<double>(operating);

            lock.unlock();

            std::this_thread::sleep_for(checkoutTime);

            lock.lock();
            total_checkout_time += operating;
            total_customer_stay_time += (getCurrentTime() - customer.arrivalTime) + operating;
            served_customers++;
        }
        if (simulationFinished) {
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
};


void Store::customerProcess(double average_items) {
    auto start1 = std::chrono::high_resolution_clock::now();

    while (!simulationFinished) {
        auto seconds = std::chrono::duration<double>((double)1 / customer_intensity);
        std::this_thread::sleep_for(seconds);

        Customer customer;
        customer.arrivalTime = getCurrentTime();
        customer.number_of_items = generateRandomNumber(0, 2 * average_items);

        if (customer_queue.size() < maximum_queue_length) {
            customer_queue.push(customer);
            total_queue_length += customer_queue.size();
        }
        else {
            unserved_customers++;
        }
    }


    auto end1 = std::chrono::high_resolution_clock::now();

};


int Store::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
};


Statics Store::printStatistics() {
    double average_queue_length = static_cast<double>(total_queue_length) / served_customers;
    double average_customer_stay_time = static_cast<double>(total_customer_stay_time) / served_customers;
    double average_checkout_time = static_cast<double>(total_checkout_time) / served_customers;
    double  average_idle_time = static_cast<double>(total_time - average_checkout_time);

    std::cout << "Served customers: " << served_customers << std::endl;
    std::cout << "Unserved customers: " << unserved_customers << std::endl;
    std::cout << "Average queue length: " << average_queue_length << std::endl;
    std::cout << "Average buyer's stay time: " << average_customer_stay_time << std::endl;
    std::cout << "Average checkout time: " << average_checkout_time << std::endl;
    std::cout << "Average idle time at checkout: " << average_idle_time << std::endl << std::endl;

    double failureProbability = static_cast<double>(unserved_customers) / (unserved_customers + served_customers);
    std::cout << "Real Probability of failure: " << failureProbability << std::endl;
    double relativeThroughput = static_cast<double>(served_customers) / (unserved_customers + served_customers);
    std::cout << "Real Relative throughput of the store: " << relativeThroughput << std::endl;
    double absoluteThroughput = static_cast<double>(served_customers) / total_time;
    std::cout << "Real Absolute throughput of the store: " << absoluteThroughput << std::endl;

    Statics stat;
    stat.absoluteThroughput = absoluteThroughput;
    stat.failureProbability = failureProbability;
    stat.relativeThroughput = relativeThroughput;
    stat.served_customers = served_customers;
    stat.unserved_customers = unserved_customers;

    return stat;
};

Statics Store::printTeoreticStatics(double intensity, double speed, int number_of_cash_registers, int max_queue_length, double average_items) {
    double r = intensity * average_items / speed;
    double  P0 = calculate_P0(number_of_cash_registers, r, max_queue_length);

    double failureProbability = calculate_Pn(number_of_cash_registers, r, P0, max_queue_length);
    std::cout << "Teoretic Probability of failure: " << failureProbability << std::endl;

    double relativeThroughput = 1 - failureProbability;
    std::cout << "Teoretic Relative throughput of the store: " << relativeThroughput << std::endl;

    double absoluteThroughput = average_items * intensity * relativeThroughput;
    std::cout << "Teoretic Absolute throughput of the store: " << absoluteThroughput << std::endl;

    Statics stat;
    stat.absoluteThroughput = absoluteThroughput;
    stat.failureProbability = failureProbability;
    stat.relativeThroughput = relativeThroughput;

    return stat;
};


double calculate_r(double lyamda, double nu) {
    return lyamda / nu;
};

double calculate_P0(int pool_count, double r, int max_length) {
    double P0 = 0;
    for (int i = 0; i <= pool_count; i++)
    {
        P0 = P0 + pow(r, i) / Factorial(i);
    }
    for (int i = 1; i <= max_length; i++)
    {
        P0 = P0 + pow(r, pool_count + i) / (Factorial(pool_count) * pow(pool_count, i));
    }
    return pow(P0, -1);
};

double calculate_Pn(int pool_count, double r, double P0, int max_length) {
    double Pn = pow(r, pool_count + max_length) / (Factorial(pool_count) * pow(pool_count, max_length)) * P0;
    return Pn;
};

double Factorial(double n) {
    double factorial = 1;
    for (int i = 1; i <= n; i++)
        factorial = factorial * i;
    return factorial;
};
