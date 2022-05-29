#include <thread>
#include <cstdlib>
#include <task.h>
#include <iostream>

int main(int argc, char* argv[]){
    // поток клиентов
    int total_time = 10000;
    double client_intensivity = (double)1 / 10;
    int one_time_period = 10;
    int cur_time;
    int client_count = 0;
    int mean_products = 10;
    int threads_count = 5;
    int deque_size = 10;
    int product_process_time = 10;
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    Server server = Server(threads_count, deque_size, product_process_time);
    server.Start();
    int clients_per_time_unit;
    int product_count;
    while(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - begin).count() < total_time){
        // ToDo: some shit going on with rand() and Illegal instruction (core dumped)
        clients_per_time_unit = rand() % int(2 * one_time_period * client_intensivity);
        for(int i = 0; i < clients_per_time_unit; i++){
            product_count  = rand() % (2 * mean_products);
            Client client = Client(client_count, product_count);
            server.add_to_queue(client);
            client_count ++;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(one_time_period));
    }
    server.Stop();
    std::cout << "Customers served: " << server.stats.customers_served << " Customers rejected: " << server.stats.customers_rejected << "\n";
    std::cout << "Mean deque length: " << server.stats.deque_len / server.stats.observations_count << "\n";
    std::cout << "Threads working time: " << server.stats.thread_working << " Threads plain time: " << server.stats.thread_plain << "\n";
    std::cout << "Mean client wait: " << server.stats.client_wait / server.stats.customers_served << "\n";
    std::cout << "Rejection probability: " << (double)server.stats.customers_rejected / (server.stats.customers_served + server.stats.customers_rejected) << "\n";
    std::cout << "Relative capacity: " << 1 - (double)server.stats.customers_rejected / (server.stats.customers_served + server.stats.customers_rejected) << "\n";
    std::cout << "Absolute capacity: " << client_intensivity * (1 - (double)server.stats.customers_rejected / (server.stats.customers_served + server.stats.customers_rejected)) << std::endl;
    return 0;
}