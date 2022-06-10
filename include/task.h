// Copyright 2022 UNN
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
    int reqFlow;
    int servedClients;
    int unservedClients;
    double averageLen = 1.0;
    double average_client_time_line_and_cass = 0.0;
    double averageWorkTime = 0.0;
    double averageDownTime = 0.0;

    int Lines = 0;
    int numCass;
    int line_len;
    int maxNumClients;
    int intensClients;
    int servSpeed;
    double averageItemNum = 0.0;
    int maxLen = 0;
    int availCasses = 0;
    double cass_downtime = 0.0;
    double cass_worktime = 0.0;
    bool finished = false;
    int ClientsChecks = 0;
    workingcasses casses;
    std::vector<std::queue<Client*>*> lines;
    std::mutex myMutex;

public:

    Client* getClient();
    void start();
    void serveClient(Client* client, int number);
    void serveLine(std::queue<Client*>* clients);
    void serveSupermarket();
    int workingCasNum();
    double getAverageLineLength();
    double getAverageWaitingTimeForClient();
    double getAverageWorkTime();
    double getAverageDownTime();
    int getRequestsFlow();
    int getAmountOfServedClients();
    int getAmountOfUnservedClients();
    explicit Market(int cas, int iC, int servSp, int avItN, int mL, int mnC);

};