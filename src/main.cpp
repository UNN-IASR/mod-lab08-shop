#include "task.h"
#include <iostream>


int main()
{
    setlocale(LC_ALL, "Rus");
    double numberRegisters = 5;
    double intensity = 30;
    double speed = 0.6;
    double avgProducts = 15;
    double maxQueue = 7;

    Shop shop(numberRegisters, intensity, speed, avgProducts, maxQueue);
    shop.Processing();
    shop.Data_statistics();
    shop.Theoretical_result();

    std::cout << "Served customers: " << shop.serviced_customers << std::endl;
    std::cout << "Unserved customers: " << shop.unservesed_customers << std::endl;
    std::cout << "Average queue length: " << shop.average_length_queue << std::endl;
    std::cout << "The average time a customer is in the queue: " << shop.average_time_customer_in_queue << std::endl;
    std::cout << "Average time spent by a customer at the checkout " << (int)shop.average_time_customer_at_cassa << std::endl;
    std::cout << "The average opening time of the cash register: " << shop.average_time_work_cassa << std::endl;
    std::cout << "Average cash register downtime: " << shop.average_time_busy_cassa << std::endl << std::endl;
    std::cout << "Practical result" << std::endl;
    std::cout << "Probability of failure: " << shop.Pre << std::endl;
    std::cout << "The relative throughput of the shop: " << shop.Q << std::endl;
    std::cout << "The absolute throughput of the shop: " << shop.A << std::endl << std::endl;

}