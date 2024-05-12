// Copyright 2024 Shalnov_Yuri
#include "Task.h"

int fact(int n) {
    if (n == 0)
        return 1;
    else {
        int result = 1;
        for (int i = 1; i <= n; i++)
            result = result * i;
        return result;
    }
}

Client::Client(int items) {
    num_items = items;
}

int Client::getItemsNum() {
    return num_items;
}

task::task(int _num_checkouts, double _lambda, int _checkout_time_ms, int _max_queue_length, int _mean_num_items) {
    num_checkouts = _num_checkouts;
    lambda = _lambda;
    checkout_time_ms = _checkout_time_ms;
    max_queue_length = _max_queue_length;
    mean_num_items = _mean_num_items;
    setClientsFinished(false);
    checkout_times.resize(num_checkouts, 0);
}

void task::setClientsFinished(bool status) {
    clients_finished = status;
}

void task::startSimulation() {
    vector<thread> checkout_threads;
    for (int i = 0; i < num_checkouts; i++) {
        checkout_threads.emplace_back([this, i]() {
            serveClients(i);
            });
        idle_checkouts.push(i);
    }

    thread client_thread([this]() {
        generateClients();
        });

    client_thread.join();
    {
        unique_lock<mutex> lock(queue_mutex);
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
        unique_lock<mutex> lock(queue_mutex);
        while (client_queue.empty()) {
            // Åñëè î÷åðåäü ïóñòà, æäåì ïîÿâëåíèÿ ïîêóïàòåëåé
            if (clients_finished) {
                return;
            }
            queue_condition.wait(lock);
        }
        Client client = (Client)std::move(client_queue.front());
        client_queue.pop();
        std::this_thread::sleep_for(std::chrono::milliseconds(client.getItemsNum() * checkout_time_ms));
        checkout_times[id] += client.getItemsNum() * checkout_time_ms;
        clients_served++;
        {
            lock_guard<mutex> lock(idle_mutex);
            idle_checkouts.push(id);
            idle_condition.notify_one();
        }
    }
}

void task::generateClients()
{
    default_random_engine generator;
    poisson_distribution<int> poisson(lambda);
    normal_distribution<double> normal_dist(mean_num_items);

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
