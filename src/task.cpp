#include "task.h"

#include <iostream>

Shop::Shop(double _countCash, double _li, double _mu, double _countProduct, double _maxQueue) {
    count_cassa = _countCash;
    intensity_customers = _li;
    mu = _mu;
    average_number_of_products = _countProduct;
    max_length_queue = _maxQueue;

    queue = std::queue<Customer>();
    cassa = std::vector<Cash>();
    number_threads = std::vector<std::thread*>();

    for (int i = 0; i < count_cassa; i++) {
        cassa.push_back(Cash());
        cassa[i].number_cassa = i;
        cassa[i].averge_opening_time_cassa = 0;
        cassa[i].averge_downtime_cassa = 0;
        cassa[i].busy = true;
        cassa[i].timeDownStart = std::chrono::system_clock::now();
    }
}

double Shop::Factorial(int n) {
    double res = (n == 0) ? 1 : n * Factorial(n - 1);
    return res;
}

void Shop::Theoretical_result() {
    double la1 = intensity_customers;
    double nu1 = 1000.0 * average_number_of_products / mu;
    double ro1 = la1 / nu1;
    double denominator_P0 = 0;
    for (int i = 0; i <= count_cassa; i++) {
        denominator_P0 += (double)pow(ro1, i) / Factorial(i);
    }
    for (int i = 1; i <= max_length_queue; i++) {
        denominator_P0 += (double)pow(ro1, count_cassa + i) / Factorial(count_cassa) * (pow(count_cassa, i));
    }
    double P0 = (double)1 / denominator_P0;
    double denominator_Pn = (double)((1 - pow(ro1 / count_cassa, max_length_queue)) / (1 - ro1 / count_cassa));
    double numerator_Pn = (double)((pow(ro1, count_cassa) / Factorial(count_cassa)));
    double Pn = P0 * (numerator_Pn / denominator_Pn);

    double denominator_Pre = (double)(pow(count_cassa, max_length_queue) * Factorial(count_cassa));
    double numerator_Pre = (double)(pow(ro1, count_cassa + max_length_queue));
    double Pre1 = P0 * (numerator_Pre / denominator_Pre);
    double Q1 = 1 - Pre1;
    double A1 = intensity_customers * Q1;

    std::cout << "Theoretical results" << std::endl;
    std::cout << "Probability of failure:" << Pre1 << std::endl;
    std::cout << "The relative throughput of the shop:" << Q1 << std::endl;
    std::cout << "The absolute throughput of the shop:" << A1 << std::endl << std::endl;

}

void Shop::Work_cassa(int id, Customer client, double speed) {
    std::unique_lock<std::mutex> mu(mutex_);

    cassa[id].averge_downtime_cassa += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - cassa[id].timeDownStart).count();
    cassa[id].averge_opening_time_cassa += client.count_products / speed;
    client.time_served = (int)(client.count_products / speed);

    std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.count_products / speed)));

    cassa[id].timeDownStart = std::chrono::system_clock::now();
    cassa[id].busy = true;

    mu.unlock();
}

void Shop::Processing() {
    allTimeStart = std::chrono::system_clock::now();

    srand(time(0));
    int time = 0;
    int nextCustomerTime = rand() % 15 + 1;
    int numberClients = 0;

    while (true) {
        if (numberClients < intensity_customers) {
            time++;
            if (time == nextCustomerTime) {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                numberClients++;
                Customer client;
                client.count_products = rand() % (2 * (int)average_number_of_products);
                client.time_start_in_queue = std::chrono::system_clock::now();

                if (queue.size() < max_length_queue) {
                    serviced_customers++;
                    queue.push(client);
                }
                else {
                    unservesed_customers++;
                }
                nextCustomerTime = time + rand() % 10 + 1;
            }
        }

        for (int i = 0; i < count_cassa; i++) {
            if ((queue.size() > 0) && (cassa[i].busy == true)) {
                cassa[i].busy = false;

                queue.front().time_waited = std::chrono::duration_cast<std::chrono::milliseconds>
                    (std::chrono::system_clock::now() - queue.front().time_start_in_queue).count();

                number_threads.push_back(new std::thread(&Shop::Work_cassa, this, i, queue.front(), mu));

                average_length_queue += queue.size();
                average_time_customer_in_queue += queue.front().time_waited;
                average_time_customer_at_cassa += queue.front().count_products / mu;
                queue.pop();
            }
        }
        if (queue.empty() && numberClients == intensity_customers) break;
    }
    for (auto& th : number_threads)
        th->join();

    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();
}


void Shop::Data_statistics() {
    average_time_work_cassa = 0;
    average_time_busy_cassa = 0;

    for (int i = 0; i < count_cassa; i++) {
        average_time_work_cassa += cassa[i].averge_opening_time_cassa;
        average_time_busy_cassa += cassa[i].averge_downtime_cassa;
    }

    average_length_queue = average_length_queue / intensity_customers;
    average_time_customer_in_queue = average_time_customer_in_queue / intensity_customers;
    average_time_customer_at_cassa = average_time_customer_at_cassa / intensity_customers;

    average_time_work_cassa = average_time_work_cassa / count_cassa;
    average_time_busy_cassa = average_time_busy_cassa / count_cassa;

    double la2 = intensity_customers / allTime;
    double nu2 = (double)serviced_customers / allTime;
    double ro2 = la2 / nu2;

    double Pre2 = unservesed_customers / intensity_customers;
    double Q2 = 1.0 - Pre2;
    double A2 = la2 * Q2;

    Pre = Pre2;
    Q = Q2;
    A = A2;

}