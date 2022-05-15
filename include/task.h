// Copyright 2022 UNN-IASR
#pragma once
#include <iostream>
#include <thread>
#include <condition_variable>
#include <functional>
#include <vector>
#include <random>
#include <queue>
#include <mutex>
#include <list>

typedef std::vector<std::thread*> workingcasses;

using namespace std;

class Client {
public:
    std::vector<int> check;
    explicit Client(std::vector<int> check1);
};

class Market {
private:
    int requests_flow;
    int served_clients; 
    int unserved_clients; 
    double average_line_len = 1.0; 
    double average_client_time_line_and_cass = 0.0; 
    double average_work_time = 0.0; 
    double average_down_time = 0.0; 

    int all_lines = 0;
    int casses_number; 
    int line_len; 
    int max_num_of_clients;
    int clients_intensity; 
    int serving_speed; 
    double average_item_num = 0.0; 
    int max_line_len = 0; 
    int working_casses = 0;
    double cass_downtime = 0.0;
    double cass_worktime = 0.0;
    bool finished = false;
    int all_checks_for_clients = 0;
    workingcasses casses_WIP;
    std::vector<std::queue<Client*>*> lines;
    std::mutex myMutex;

public:

    Client* getClient();
    void start();
    void serveClient(Client* client, int number);
    void serveLine(std::queue<Client*>* clients);
    void serveSupermarket();
    int workingcassesNumber();
    double getAverageLineLength();
    double getAverageWaitingTimeForClient();
    double getAverageWorkTime();
    double getAverageDownTime();
    int getRequestsFlow();
    int getAmountOfServedClients();
    int getAmountOfUnservedClients();
    explicit Market(int casses, int clients_intensity, int serving_speed, int average_item_num, int max_line_len, int max_num_of_clients);

};
