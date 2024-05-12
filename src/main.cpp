// Copyright 2024 alenapoliakova
#include "task.h"

int main() {
    int number_cash_registers = 2;
    int customer_intensity = 10;
    double speed = 10;
    int max_queue_length = 10;
    int simulation_time = 20;
    double average_items = 3;
    Store store(number_cash_registers, customer_intensity, speed, max_queue_length);

    store.simulate(simulation_time, average_items);

    store.printStatistics();
    store.printTeoreticStatics(customer_intensity, speed, number_cash_registers, max_queue_length, average_items);
}
