#include "task.h"
Client::Client(int items) : num_items(items) {}

task::task(int num_checkouts, double lambda, int checkout_time_ms, int max_queue_length, int mean_num_items) :
    num_checkouts(num_checkouts), lambda(lambda), checkout_time_ms(checkout_time_ms),
    max_queue_length(max_queue_length), mean_num_items(mean_num_items), clients_finished(false) {
    checkout_times.resize(num_checkouts, 0);
    
}

void task::startSimulation() {
    std::vector<std::thread> checkout_threads;
    for (int i = 0; i < num_checkouts; i++) {
        checkout_threads.emplace_back([this, i]() {
            serveClients(i);
            });
        idle_checkouts.push(i);
    }

    std::thread client_thread([this]() {
        generateClients();
        });

    client_thread.join();
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        clients_finished = true;
        queue_condition.notify_all();
    }
    for (auto& thread : checkout_threads) {
        thread.join();
    }
    for (int i = 0; i < num_checkouts; i++) {
        total_time += checkout_times[i];
        idle_time += (checkout_threads.size() - idle_checkouts.size()) * checkout_time_ms;
    }
}

void task::serveClients(int id)
{
    while (true) {
        std::unique_lock<std::mutex> lock(queue_mutex);
        while (client_queue.empty()) {
            // Если очередь пуста, ждем появления покупателей
            if (clients_finished)
                return;
            queue_condition.wait(lock);
        }
        Client client = (Client)std::move(client_queue.front());
        client_queue.pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(client.num_items * checkout_time_ms));
        checkout_times[id] += client.num_items * checkout_time_ms;
        clients_served++;

        {
            std::lock_guard<std::mutex> lock(idle_mutex);
            idle_checkouts.push(id);
            idle_condition.notify_one();
        }
    }
}

void task::generateClients()
{
    std::default_random_engine generator;
    std::poisson_distribution<int> poisson(lambda);
    std::normal_distribution<double> normal_dist(mean_num_items);

    for (int i = 0; i < 100; i++)
    {
        int wait_time = poisson(generator) * 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_time));

        int num_items = std::round(normal_dist(generator));

        Client client(num_items);
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            if (client_queue.size() >= max_queue_length) {
                rejected_clients++;
                continue;
            }
            client_queue.push(std::move(client));
            queue_condition.notify_one();
        }
    }
}
