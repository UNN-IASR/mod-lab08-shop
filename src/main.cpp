#include <iostream>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <random>
#include <chrono>
#include <atomic>
#include <iomanip>

using namespace std;
using namespace chrono;

struct Customer {
    int id;
    int items;
    system_clock::time_point arrival_time;
};

class Cashier {
public:
    Cashier(int id, int processing_speed)
        : id(id), processing_speed(processing_speed), busy(false), total_work_time(0), total_idle_time(0), last_end_time(system_clock::now()) {}

    void serveCustomer(Customer customer) {
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

    void waitForCustomer() {
        unique_lock<mutex> lock(mx);
        while (busy) {
            cv.wait(lock);
        }
    }

    int getId() const { return id; }
    bool isBusy() const { return busy; }
    long getTotalWorkTime() const { return total_work_time; }
    long getTotalIdleTime() const { return total_idle_time; }

private:
    int id;
    int processing_speed;
    atomic<bool> busy;
    long total_work_time;
    long total_idle_time;
    system_clock::time_point last_end_time;
    mutex mx;
    condition_variable cv;
};

class Supermarket {
public:
    Supermarket(int num_cashiers, int customer_flow_rate, int processing_speed, int avg_items_per_cart, int max_queue_length)
        : customer_flow_rate(customer_flow_rate), avg_items_per_cart(avg_items_per_cart), max_queue_length(max_queue_length), total_queue_length(0), total_customers(0), simulation_finished(false) {
        for (int i = 0; i < num_cashiers; ++i) {
            cashiers.emplace_back(make_unique<Cashier>(i, processing_speed));
        }
    }

    void runSimulation(int duration_s) {
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
    }

private:
    vector<unique_ptr<Cashier>> cashiers;
    queue<Customer> customer_queue;
    mutex queue_mx;
    condition_variable queue_cv;
    atomic<int> customers_served{ 0 };
    atomic<int> customers_not_served{ 0 };
    int customer_flow_rate;
    int avg_items_per_cart;
    int max_queue_length;
    atomic<long> total_queue_length;
    atomic<int> total_customers;
    bool simulation_finished;

    void generateCustomers(system_clock::time_point end_time) {
        int customer_id = 0;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> item_dist(1, avg_items_per_cart * 2);
        uniform_int_distribution<> arrival_dist(1, 2);

        while (system_clock::now() < end_time) {
            this_thread::sleep_for(seconds(arrival_dist(gen)));
            Customer customer{ customer_id++, item_dist(gen), system_clock::now() };

            unique_lock<mutex> lock(queue_mx);
            if (customer_queue.size() < max_queue_length) {
                customer_queue.push(customer);
                total_customers++;
                total_queue_length += customer_queue.size();
                cout << "Покупатель " << customer.id << " с " << customer.items << " товарами пришел в магазин." << endl;
                queue_cv.notify_one();
            }
            else {
                customers_not_served++;
                cout << "Покупатель " << customer.id << " ушел, так как очередь полная." << endl;
            }
        }
    }

    void operateCashier(Cashier* cashier) {
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

    void printStatistics() {
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
};

int main() {
    setlocale(LC_ALL, "RUSSIAN");
    Supermarket supermarket(5, 10, 1, 10, 20);
    supermarket.runSimulation(60); // Run simulation for 60 seconds
    return 0;
}
