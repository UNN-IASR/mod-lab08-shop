// Copyright 2024 mpv-enjoyer
#include <math.h>
#include <gtest/gtest.h>
#include <iostream>
#include <random>
#include <queue>
#include <set>
#include "task.h"

float factorial(const int n) {
    float f = 1;
    for (int i = 1; i <= n; ++i)
        f *= i;
    return f;
}

int main() {
    const int cashbox_count = 5;
    const float service_time = 3.0f;
    const int task_count = 300;
    const float avg_task_delay = 1.0f;
    const float avg_task_complexity = 2.5f;
    const int queue_size = 5;

    float avg_queue_length = 0;

    std::vector<Task> tasks;
    std::queue<Task> queue;
    float current_time = 0.0f;

    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution
        (1, avg_task_complexity * 2);
    auto new_complexity = std::bind(distribution, generator);

    std::uniform_real_distribution<> distribution2(0.0f, 1.0f);
    auto new_task_delay_coeff = std::bind(distribution2, generator);

    std::set<float> queue_updates;

    for (int current_task = 0; current_task < task_count; current_task++) {
        int complexity = new_complexity();
        int time = service_time * complexity;
        Task new_task = Task(time, current_time);
        int cashbox = Task::find_free(cashbox_count, current_time, tasks);
        bool assign = cashbox != -1;
        if (assign) {
            new_task.assign(current_time, cashbox);
            tasks.push_back(new_task);
            queue_updates.insert(current_time + time);
        } else {
            if (queue.size() == queue_size) {
                tasks.push_back(new_task);
            } else {
                queue.push(new_task);
            }
        }
        float task_delay = new_task_delay_coeff() * 2 * avg_task_delay;
        float new_time = current_time + task_delay;
        while (*(queue_updates.begin()) <= new_time) {
            float diff = *(queue_updates.begin()) - current_time;
            avg_queue_length += diff * queue.size();

            current_time = *(queue_updates.begin());
            queue_updates.erase(current_time);
            int cashbox = -1;
            for (int i = 0; i < queue.size(); i++) {
                cashbox = Task::find_free(cashbox_count, current_time, tasks);
                if (cashbox == -1) break;
                Task task = queue.front();
                queue.pop();
                task.assign(current_time, cashbox);
                tasks.push_back(task);
                queue_updates.insert(current_time + task.time);
            }
        }
        float diff = new_time - current_time;
        avg_queue_length += diff * queue.size();
        current_time = new_time;

        cashbox = -1;
        for (int i = 0; i < queue.size(); i++) {
            cashbox = Task::find_free(cashbox_count, current_time, tasks);
            if (cashbox == -1) break;
            Task task = queue.front();
            queue.pop();
            task.assign(current_time, cashbox);
            tasks.push_back(task);
            queue_updates.insert(current_time + task.time);
        }
    }
    while (queue_updates.size() != 0) {
        current_time = *(queue_updates.begin());
        queue_updates.erase(current_time);
        int cashbox = -1;
        for (int i = 0; i < queue.size(); i++) {
            float diff = *(queue_updates.begin()) - current_time;
            avg_queue_length += diff * queue.size();

            cashbox = Task::find_free(cashbox_count, current_time, tasks);
            if (cashbox == -1) break;
            Task task = queue.front();
            queue.pop();
            task.assign(current_time, cashbox);
            tasks.push_back(task);
            queue_updates.insert(current_time + task.time);
        }
    }

    float stat_avg_queue_length = avg_queue_length / current_time;
    int   stat_not_assigned     = 0;
    int   stat_assigned         = 0;
    float stat_avg_queue_wait   = 0;
    float stat_avg_task_wait    = 0;
    float stat_avg_cashbox_work = 0;
    float stat_avg_cashbox_wait = 0;

    float coefficient = 1.0f / tasks.size();
    float work_sum = 0;
    float wait_sum = tasks.size() * current_time;
    for (Task task : tasks) {
        if (!task.is_assigned()) {
            stat_not_assigned++;
            continue;
        }
        stat_assigned++;
        stat_avg_queue_wait += coefficient * (task.begin - task.begin_begin);
        stat_avg_task_wait += coefficient * task.time;

        work_sum += task.time;
        wait_sum -= task.time;
    }
    stat_avg_cashbox_work = work_sum / cashbox_count;
    stat_avg_cashbox_wait = wait_sum / cashbox_count;

    std::cout << "stat_avg_queue_length : " << stat_avg_queue_length << "\n";
    std::cout << "stat_not_assigned     : " << stat_not_assigned     << "\n";
    std::cout << "stat_assigned         : " << stat_assigned         << "\n";
    std::cout << "stat_avg_queue_wait   : " << stat_avg_queue_wait   << "\n";
    std::cout << "stat_avg_task_wait    : " << stat_avg_task_wait    << "\n";
    std::cout << "stat_avg_cashbox_work : " << stat_avg_cashbox_work << "\n";
    std::cout << "stat_avg_cashbox_wait : " << stat_avg_cashbox_wait << "\n";

    std::cout << " --- Real --- " << "\n";
    float inbetween_stat_not_assigned = stat_not_assigned;
    float denial_probability = inbetween_stat_not_assigned / task_count;
    float relative_throughput = 1.0f - denial_probability;
    float absolute_throughput = task_count*relative_throughput/current_time;
    std::cout << "probability of denial : " << denial_probability    << "\n";
    std::cout << "relative throughput   : " << relative_throughput   << "\n";
    std::cout << "absolute throughput   : " << absolute_throughput   << "\n";

    std::cout << " --- Theoretical --- " << "\n";

    float avg_service_time = service_time * avg_task_complexity;
    float intensivity = avg_service_time / avg_task_delay;
    float r_denial_probability = 1;
    for (int i = 1; i <= cashbox_count; i++) {
        r_denial_probability += std::pow(intensivity, i) / factorial(i);
    }
    float last_coeff;
    for (int i = cashbox_count + 1; i <= cashbox_count + queue_size; i++) {
        float current = std::pow(intensivity, i) / factorial(cashbox_count);
        last_coeff = current / std::pow(cashbox_count, i - cashbox_count);
        r_denial_probability += last_coeff;
    }
    r_denial_probability = (1.0f / r_denial_probability) * last_coeff;

    float r_relative_throughput = 1.0f - r_denial_probability;
    float r_absolute_throughput = r_relative_throughput * avg_task_delay;
    std::cout << "probability of denial : " << r_denial_probability  << "\n";
    std::cout << "relative throughput   : " << r_relative_throughput << "\n";
    std::cout << "absolute throughput   : " << r_absolute_throughput << "\n";
    return 0;
}
