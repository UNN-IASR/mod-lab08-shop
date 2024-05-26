// Copyright 2021 GHA Test Team
#include "task.h"
#include <iostream>
#include <cmath>
#include <iomanip>

Customer::Customer(int id, int num_items) : id(id), num_items(num_items) {}

Checkout::Checkout(int id) : id(id), busy(false), total_served(0), total_idle_time(0), total_service_time(0) {
    start_idle = std::chrono::steady_clock::now();
}

void Checkout::serveCustomer(Customer customer, int processing_speed) {
    busy = true;
    total_served++;
    auto start_service = std::chrono::steady_clock::now();
    total_idle_time += std::chrono::duration_cast<std::chrono::seconds>(start_service - start_idle).count();

    std::this_thread::sleep_for(std::chrono::seconds(customer.num_items * processing_speed));
    
    auto end_service = std::chrono::steady_clock::now();
    total_service_time += std::chrono::duration_cast<std::chrono::seconds>(end_service - start_service).count();

    busy = false;
    start_idle = std::chrono::steady_clock::now();
}

Supermarket::Supermarket(int num_checkouts, int customer_arrival_rate, int processing_speed, int avg_cart_size, int max_queue_length) :
    num_checkouts(num_checkouts),
    customer_arrival_rate(customer_arrival_rate),
    processing_speed(processing_speed),
    avg_cart_size(avg_cart_size),
    max_queue_length(max_queue_length),
    customers_served(0),
    customers_not_served(0),
    total_queue_time(0),
    total_customers_in_queue(0) {

    for (int i = 0; i < num_checkouts; ++i) {
        checkouts.push_back(std::make_unique<Checkout>(i));
    }
}

void Supermarket::customerGenerator() {
    std::default_random_engine generator;
    std::poisson_distribution<int> arrival_distribution(customer_arrival_rate);
    std::poisson_distribution<int> items_distribution(avg_cart_size);

    int customer_id = 0;

    while (true) {
        int sleep_time = arrival_distribution(generator);
        std::this_thread::sleep_for(std::chrono::seconds(sleep_time));
        int num_items = items_distribution(generator);
        Customer new_customer(customer_id++, num_items);
        std::unique_lock<std::mutex> lock(queue_mutex);
        if (queue.size() < max_queue_length) {
            queue.push(new_customer);
            total_customers_in_queue++;
            cv.notify_one();
        } else {
            customers_not_served++;
        }
    }
}

void Supermarket::checkoutWorker(Checkout& checkout) {
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        cv.wait(lock, [this] { return !queue.empty(); });
        if (!queue.empty()) {
            Customer customer = queue.front();
            queue.pop();
            lock.unlock();

            auto start_queue_time = std::chrono::steady_clock::now();

            checkout.serveCustomer(customer, processing_speed);

            auto end_queue_time = std::chrono::steady_clock::now();
            total_queue_time += std::chrono::duration_cast<std::chrono::seconds>(end_queue_time - start_queue_time).count();

            customers_served++;
        }
    }
}

void Supermarket::runSimulation() {
    std::vector<std::thread> checkout_threads;
    for (int i = 0; i < num_checkouts; ++i) {
        checkout_threads.emplace_back(&Supermarket::checkoutWorker, this, std::ref(*checkouts[i]));
    }
    std::thread customer_thread(&Supermarket::customerGenerator, this);

    customer_thread.detach();
    for (auto& t : checkout_threads) {
        t.detach();
    }
}

void Supermarket::printStatistics() {
    int total_idle_time = 0;
    int total_service_time = 0;

    for (const auto& checkout : checkouts) {
        total_idle_time += checkout->total_idle_time;
        total_service_time += checkout->total_service_time;
    }

    int total_customers = customers_served.load() + customers_not_served.load();
    double Prej = static_cast<double>(customers_not_served.load()) / total_customers;
    double Q = 1 - Prej;
    double A = customer_arrival_rate * Q;

    std::cout << "Обслужено покупателей: " << customers_served.load() << std::endl;
    std::cout << "Не обслужено покупателей: " << customers_not_served.load() << std::endl;
    std::cout << "Средняя длина очереди: " << (total_customers_in_queue == 0 ? 0 : (float)total_customers_in_queue / total_customers) << std::endl;
    std::cout << "Среднее время в очереди и на кассе: " << (customers_served == 0 ? 0 : (float)total_queue_time / customers_served) << " секунд" << std::endl;
    std::cout << "Среднее время обслуживания на кассе: " << (num_checkouts == 0 ? 0 : (float)total_service_time / num_checkouts) << " секунд" << std::endl;
    std::cout << "Среднее время простоя кассы: " << (num_checkouts == 0 ? 0 : (float)total_idle_time / num_checkouts) << " секунд" << std::endl;

    std::cout << "Вероятность отказа (Prej): " << Prej << std::endl;
    std::cout << "Относительная пропускная способность (Q): " << Q << std::endl;
    std::cout << "Абсолютная пропускная способность (A): " << A << std::endl;

    // Теоретические вычисления
    double P0 = calculateTheoreticalP0();
    double Prej_theoretical = calculateTheoreticalPrej(P0);
    double Q_theoretical = calculateTheoreticalQ(Prej_theoretical);
    double A_theoretical = calculateTheoreticalA(Q_theoretical);

    std::cout << "Теоретическая вероятность отказа (Prej): " << Prej_theoretical << std::endl;
    std::cout << "Теоретическая относительная пропускная способность (Q): " << Q_theoretical << std::endl;
    std::cout << "Теоретическая абсолютная пропускная способность (A): " << A_theoretical << std::endl;
}

double Supermarket::calculateTheoreticalP0() {
    double rho = static_cast<double>(customer_arrival_rate) / (num_checkouts * processing_speed);
    double P0 = 0.0;

    // Вычисление P0
    for (int k = 0; k <= num_checkouts; ++k) {
        P0 += pow(rho, k) / tgamma(k + 1);
    }
    double sum_rho_n = 0.0;
    for (int j = 1; j <= max_queue_length; ++j) {
        sum_rho_n += pow(rho, j) / tgamma(j + 1);
    }
    P0 += (pow(rho, num_checkouts) / tgamma(num_checkouts + 1)) * sum_rho_n;
    P0 = 1.0 / P0;

    return P0;
}

double Supermarket::calculateTheoreticalPrej(double P0) {
    double rho = static_cast<double>(customer_arrival_rate) / (num_checkouts * processing_speed);
    return (pow(rho, num_checkouts + max_queue_length) / (tgamma(num_checkouts + 1) * tgamma(max_queue_length + 1))) * P0;
}

double Supermarket::calculateTheoreticalQ(double Prej) {
    return 1 - Prej;
}

double Supermarket::calculateTheoreticalA(double Q) {
    return customer_arrival_rate * Q;
}
