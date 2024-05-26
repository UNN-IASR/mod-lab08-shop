// Copyright 2021 GHA Test Team
#include "task.h"
#include <thread>
#include <chrono>
#include <iostream>

int main() {
    int num_checkouts = 5;
    int customer_arrival_rate = 1; // покупателей в секунду
    int processing_speed = 1; // секунд на товар
    int avg_cart_size = 10; // среднее количество товаров
    int max_queue_length = 10;

    Supermarket supermarket(num_checkouts, customer_arrival_rate, processing_speed, avg_cart_size, max_queue_length);

    std::thread simulation_thread(&Supermarket::runSimulation, &supermarket);

    std::this_thread::sleep_for(std::chrono::seconds(60)); // запустить симуляцию на 60 секунд

    supermarket.printStatistics();

    simulation_thread.detach();

    return 0;
}
