// Copyright 2022 UNN-IASR
#include "../include/task.h"

using namespace std;

Market::Market(int cass_number1, int max_num_of_clients1, int clients_intensity1, int serving_speed1, int average_item_num1, int max_line_len1) {
    casses_number = cass_number1;
    max_num_of_clients = max_num_of_clients1;
    clients_intensity = clients_intensity1;
    serving_speed = serving_speed1;
    average_item_num = average_item_num1;
    max_line_len = max_line_len1;
}


Client::Client(std::vector<int> check1) {
    check = check1;
}


double Market::getAverageLineLength() {
    return average_line_len;
}

double Market::getAverageWaitingTimeForClient() {
    double result = 0;
    for (int i = 1; i <= average_line_len; i++) {
        result += i * static_cast<double>(average_item_num * serving_speed);
    }
    return result / average_line_len;
}

double Market::getAverageWorkTime() {
    return average_work_time;
}

double Market::getAverageDownTime() {
    return average_down_time;
}

int Market::workingcassesNumber() {
    return casses_WIP.size();
}

int Market::getRequestsFlow() {
    return requests_flow;
}

int Market::getAmountOfServedClients() {
    return served_clients;
}

int Market::getAmountOfUnservedClients() {
    return unserved_clients;
}

Client* Market::getClient() {
    std::vector<int> check(average_item_num);
    for (int i = 0; i < average_item_num; i++) {
        check[i] = std::rand() % 100 + 1;
    }
    return new Client(check);
}

void Market::start() {
    serveSupermarket();
    for (auto current_line : casses_WIP) {
        current_line->join();
    }
    average_line_len = static_cast<double>(all_lines / all_checks_for_clients);
}

void Market::serveClient(Client* client1, int number) {
    for (auto i = 0; i < client1->check.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(serving_speed));
        std::unique_lock<std::mutex> my_lock(myMutex);
        average_work_time += static_cast<double>(serving_speed * static_cast<std::int64_t>(working_casses) / static_cast<double>(casses_number));
        average_down_time += static_cast<double>(serving_speed * (casses_number - static_cast<std::int64_t>(working_casses)) / static_cast<double>(casses_number));
        std::cout << "cass N" << std::this_thread::get_id()
            << " client: " << number << " item: " << i + 1 << "\n";
        my_lock.unlock();
    }
    served_clients++;
}

void Market::serveLine(std::queue<Client*>* clients1) {
    int number_of_served_clients = 1;
    while (!finished) {
        if (!clients1->empty()) {
            int count = 0;
            int iteration = 0;
            std::queue<int>* prevs = new std::queue<int>();
            while (!clients1->empty()) {
                auto client = clients1->front();
                serveClient(client, number_of_served_clients);
                clients1->pop();
                count++;
                number_of_served_clients++;
                iteration++;
            }
            std::unique_lock<std::mutex> my_lock(myMutex);
            all_lines += count;
            all_checks_for_clients++;
            my_lock.unlock();
        }
    }
}

void Market::serveSupermarket() {
    srand(time(nullptr));
    int active_lines = 0;
    for (int i = 0; i < max_num_of_clients; i++) {
        requests_flow++;
        working_casses = 0;
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() > 0) {
                working_casses++;
            }
        }
        bool free_line = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(clients_intensity));
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() < line_len) {
                (*it)->push(getClient());
                free_line = true;
                break;
            }
        }
        if (!free_line) {
            if (active_lines < casses_number) {
                active_lines++;
                auto new_line = new std::queue <Client*>;
                new_line->push(getClient());
                lines.push_back(new_line);
                casses_WIP.push_back(new std::thread(&Market::serveLine, this, new_line));
            }
            else {
                unserved_clients++;
            }
        }
    }
    finished = true;
}
