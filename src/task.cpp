#include "../include/task.h"
#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;
using namespace chrono;

Cashier::Cashier(int id, int processing_speed)
    : id(id), processing_speed(processing_speed), busy(false), total_work_time(0), total_idle_time(0), last_end_time(system_clock::now()) {}

void Cashier::serveCustomer(Customer customer) {
    unique_lock<mutex> lock(mx);
    busy = true;
    auto start_time = system_clock::now();
    auto idle_time = duration_cast<seconds>(start_time - last_end_time).count();
    total_idle_time += idle_time;
    cout << "Касса " << id << " начинает обслуживание покупателя " << customer.id << " с " << customer.items << " товарами." << endl;
    this_thread::sleep_for(seconds(customer.items * processing_speed));
    auto end_time = system_clock::now();
    busy = false;
    total_work_time += duration_cast<seconds>(end_time - start_time).count();
    last_end_time = end_time;
    cout << "Касса " << id << " закончила обслуживание покупателя " << customer.id << endl;
    cv.notify_one();
}

void Cashier::waitForCustomer() {
    unique_lock<mutex> lock(mx);
    while (busy) {
        cv.wait(lock);
    }
}

int Cashier::getId() const {
    return id;
}

bool Cashier::isBusy() const {
    return busy;
}

long Cashier::getTotalWorkTime() const {
    return total_work_time;
}

long Cashier::getTotalIdleTime() const {
    return total_idle_time;
}

Supermarket::Supermarket(int num_cashiers, int customer_flow_rate, int processing_speed, int avg_items_per_cart, int max_queue_length)
    : customer_flow_rate(customer_flow_rate), avg_items_per_cart(avg_items_per_cart), max_queue_length(max_queue_length), total_queue_length(0), total_customers(0), simulation_finished(false) {
    for (int i = 0; i < num_cashiers; ++i) {
        cashiers.emplace_back(make_unique<Cashier>(i, processing_speed));
    }
}

void Supermarket::runSimulation(int duration_s) {
    auto end_time = system_clock::now() + seconds(duration_s);
    thread customer_thread(&Supermarket::generateCustomers, this, end_time);
    vector<thread> cashier_threads;
    for (auto& cashier : cashiers) {
        cashier_threads.emplace_back(&Supermarket::operateCashier, this, cashier.get());
    }

    customer_thread.join();
    {
        unique_lock<mutex> lock(queue_mx);
        simulation_finished = true;
        queue_cv.notify_all();
    }
    for (auto& t : cashier_threads) {
        t.join();
    }

    printStatistics();
    printTheoreticalValues();
}

void Supermarket::generateCustomers(system_clock::time_point end_time) {
    int customer_id = 0;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> item_dist(1, avg_items_per_cart * 2);
    uniform_int_distribution<> arrival_dist(1, 2);

    while (system_clock::now() < end_time) {
        this_thread::sleep_for(seconds(arrival_dist(gen)));
        Customer customer{customer_id++, item_dist(gen), system_clock::now()};

        unique_lock<mutex> lock(queue_mx);
        if (customer_queue.size() < max_queue_length) {
            customer_queue.push(customer);
            total_customers++;
            total_queue_length += customer_queue.size();
            cout << "Покупатель " << customer.id << " с " << customer.items << " товарами пришел в магазин." << endl;
            queue_cv.notify_one();
        } else {
            customers_not_served++;
            cout << "Покупатель " << customer.id << " ушел, так как очередь полная." << endl;
        }
    }
}

void Supermarket::operateCashier(Cashier* cashier) {
    while (true) {
        Customer customer;
        {
            unique_lock<mutex> lock(queue_mx);
            while (customer_queue.empty() && !simulation_finished) {
                queue_cv.wait(lock);
            }
            if (simulation_finished && customer_queue.empty()) {
                break;
            }
            customer = customer_queue.front();
            customer_queue.pop();
            cout << "Покупатель " << customer.id << " подошел к кассе " << cashier->getId() << endl;
        }

        cashier->serveCustomer(customer);
        customers_served++;
    }
}

void Supermarket::printStatistics() const {
    cout << "Обслуженные покупатели: " << customers_served << endl;
    cout << "Необслуженные покупатели: " << customers_not_served << endl;
    cout << "Средняя длина очереди: " << static_cast<double>(total_queue_length) / total_customers << endl;

    double rejection_probability = static_cast<double>(customers_not_served) / (customers_served + customers_not_served);
    double relative_throughput = 1 - rejection_probability;
    double absolute_throughput = customer_flow_rate * relative_throughput;

    cout << fixed << setprecision(2);
    cout << "Вероятность отказа: " << rejection_probability << endl;
    cout << "Относительная пропускная способность: " << relative_throughput << endl;
    cout << "Абсолютная пропускная способность: " << absolute_throughput << " покупателей в секунду" << endl;

    for (const auto& cashier : cashiers) {
        cout << "Касса " << cashier->getId() << " - Общее время работы: " << cashier->getTotalWorkTime()
             << " секунд, Общее время простоя: " << cashier->getTotalIdleTime() << " секунд" << endl;
    }
}

void Supermarket::printTheoreticalValues() const {
    double lambda = static_cast<double>(customer_flow_rate);  // интенсивность потока покупателей (заявки на обслуживание)
    double mu = static_cast<double>(cashiers.size()) / avg_items_per_cart; // интенсивность обслуживания всех касс (число товаров в секунду)
    double rho = lambda / mu;                                // интенсивность нагрузки канала
    int n = cashiers.size();                                 // количество касс
    int m = max_queue_length;                                // максимальная длина очереди

    // Вероятность, что все каналы свободны
    double P0_sum = 0.0;
    for (int k = 0; k < n; ++k) {
        P0_sum += pow(rho, k) / tgamma(k + 1);
    }
    P0_sum += (pow(rho, n) / tgamma(n + 1)) * (1.0 - pow(rho / n, m + 1)) / (1.0 - rho / n);
    double P0 = 1.0 / P0_sum;

    // Вероятность отказа
    double Prej = (pow(rho, n + m) / (tgamma(n + 1) * pow(n, m))) * P0;

    // Относительная пропускная способность
    double Q = 1 - Prej;

    // Абсолютная пропускная способность
    double A = lambda * Q;

    cout << "\nТеоретические значения:\n";
    cout << "Вероятность отказа (теоретическая): " << Prej << endl;
    cout << "Относительная пропускная способность (теоретическая): " << Q << endl;
    cout << "Абсолютная пропускная способность (теоретическая): " << A << " покупателей в секунду" << endl;
}

int Supermarket::getCustomersServed() const {
    return customers_served;
}

int Supermarket::getCustomersNotServed() const {
    return customers_not_served;
}

const std::vector<std::unique_ptr<Cashier>>& Supermarket::getCashiers() const {
    return cashiers;
}