// Copyright 2022 UNN-IASR
#include "../include/task.h"



using namespace std;

Supermarket::Supermarket(int cashboxes_number1, int max_num_of_customers1, int customers_intensity1, int serving_speed1, int average_product_num1, int max_line_len1) {
    cashboxes_number = cashboxes_number1;
    max_num_of_customers = max_num_of_customers1;
    customers_intensity = customers_intensity1;
    serving_speed = serving_speed1;
    average_product_num = average_product_num1;
    max_line_len = max_line_len1;
}


Customer::Customer(std::vector<int> check1) {
    check = check1;
}


double Supermarket::getAverageLineLength() {
    return average_line_len;
}

double Supermarket::getAverageWaitingTimeForCustomer() {
    double result = 0;
    for (int i = 1; i <= average_line_len; i++) {
        result += i * static_cast<double>(average_product_num * serving_speed);
    }
    return result / average_line_len;
}

double Supermarket::getAverageWorkTime() {
    return average_work_time;
}

double Supermarket::getAverageDownTime() {
    return average_down_time;
}

int Supermarket::workingCashboxesNumber() {
    return cashboxes_WIP.size();
}

int Supermarket::getRequestsFlow(){
    return requests_flow;
}

int Supermarket::getAmountOfServedCustomers() {
    return served_customers;
}

int Supermarket::getAmountOfUnservedCustomers() {
    return unserved_customers;
}

Customer* Supermarket::getCustomer() {
    std::vector<int> check(average_product_num);
    for (int i = 0; i < average_product_num; i++) {
        check[i] = std::rand() % 100 + 1;
    }
    return new Customer(check);
}

void Supermarket::start() {
    serveSupermarket();
    for (auto current_line : cashboxes_WIP) {
        current_line->join();
    }
    average_line_len = static_cast<double>(all_lines / all_checks_for_customers);
}

void Supermarket::serveCustomer(Customer* customer1, int number) {
    for (auto i = 0; i < customer1->check.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(serving_speed));
        std::unique_lock<std::mutex> my_lock(myMutex);
        average_work_time += static_cast<double>(serving_speed * static_cast<std::int64_t>(working_cashboxes) / static_cast<double>(cashboxes_number));
        average_down_time += static_cast<double>(serving_speed * (cashboxes_number - static_cast<std::int64_t>(working_cashboxes)) / static_cast<double>(cashboxes_number));
        std::cout << "CASHBOX #" << std::this_thread::get_id()
            << " CUSTOMER: " << number << " PRODUCT: " << i + 1 << "\n";
        my_lock.unlock();
    }
    served_customers++;
}

void Supermarket::serveLine(std::queue<Customer*>* customers1) {
    int number_of_served_customers = 1;
    while (!finished) {
        if (!customers1->empty()) {
            int count = 0;
            int iteration = 0;
            std::queue<int>* prevs = new std::queue<int>();
            while (!customers1->empty()) {
                auto customer = customers1->front();
                serveCustomer(customer, number_of_served_customers);
                customers1->pop();
                count++;
                number_of_served_customers++;
                iteration++;
            }
            std::unique_lock<std::mutex> my_lock(myMutex);
            all_lines += count;
            all_checks_for_customers++;
            my_lock.unlock();
        }
    }
}

void Supermarket::serveSupermarket() {
    srand(time(nullptr));
    int active_lines = 0;
    for (int i = 0; i < max_num_of_customers; i++) {
        requests_flow++;
        working_cashboxes = 0;
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() > 0) {
                working_cashboxes++;
            }
        }
        bool free_line = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(customers_intensity));
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() < line_len) {
                (*it)->push(getCustomer());
                free_line = true;
                break;
            }
        }
        if (!free_line) {
            if (active_lines < cashboxes_number) {
                active_lines++;
                auto new_line = new std::queue <Customer*>;
                new_line->push(getCustomer());
                lines.push_back(new_line);
                cashboxes_WIP.push_back(new std::thread(&Supermarket::serveLine, this, new_line));
            }
            else {
                unserved_customers++;
            }
        }
    }
    finished = true;
} 