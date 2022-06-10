// Copyright 2022 UNN
#include "task.h"
#include <iostream>

using namespace std;

Market::Market(int nC, int mnC, int iC, int servSp, int avItN, int mL) {
    numCass = nC;
    maxNumClients = mnC;
    intensClients = iC;
    servSpeed = servSp;
    averageItemNum = avItN;
    maxLen = mL;
}


Client::Client(std::vector<int> ch) {
    check = ch;
}


double Market::getAverageLineLength() {
    return averageLen;
}

double Market::getAverageWaitingTimeForClient() {
    double result = 0;
    for (int i = 1; i <= averageLen; i++) {
        result += i * static_cast<double>(averageItemNum * servSpeed);
    }
    return result / averageLen;
}

double Market::getAverageWorkTime() {
    return averageWorkTime;
}

double Market::getAverageDownTime() {
    return averageDownTime;
}

int Market::workingCasNum() {
    return casses.size();
}

int Market::getRequestsFlow() {
    return reqFlow;
}

int Market::getAmountOfServedClients() {
    return servedClients;
}

int Market::getAmountOfUnservedClients() {
    return unservedClients;
}

Client* Market::getClient() {
    std::vector<int> check(averageItemNum);
    for (int i = 0; i < averageItemNum; i++) {
        check[i] = std::rand() % 100 + 1;
    }
    return new Client(check);
}

void Market::start() {
    serveSupermarket();
    for (auto curr : casses) {
        curr->join();
    }
    averageLen = static_cast<double>(Lines / ClientsChecks);
}

void Market::serveClient(Client* cl, int num) {
    for (auto i = 0; i < cl->check.size(); i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(servSpeed));
        std::unique_lock<std::mutex> my_lock(myMutex);
        averageWorkTime += static_cast<double>(servSpeed * static_cast<std::int64_t>(availCasses) / static_cast<double>(numCass));
        averageDownTime += static_cast<double>(servSpeed * (numCass - static_cast<std::int64_t>(availCasses)) / static_cast<double>(numCass));
        std::cout << "Касса #" << std::this_thread::get_id()
            << " клиент: " << num << " позиция: " << i + 1 << "\n";
        my_lock.unlock();
    }
    servedClients++;
}

void Market::serveLine(std::queue<Client*>* cl) {
    int numServClients = 1;
    while (!finished) {
        if (!cl->empty()) {
            int count = 0;
            int iteration = 0;
            std::queue<int>* prevs = new std::queue<int>();
            while (!cl->empty()) {
                auto client = cl->front();
                serveClient(client, numServClients);
                cl->pop();
                count++;
                numServClients++;
                iteration++;
            }
            std::unique_lock<std::mutex> my_lock(myMutex);
            Lines += count;
            ClientsChecks++;
            my_lock.unlock();
        }
    }
}

void Market::serveSupermarket() {
    srand(time(nullptr));
    int activeLines = 0;
    for (int i = 0; i < maxNumClients; i++) {
        reqFlow++;
        availCasses = 0;
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() > 0) {
                availCasses++;
            }
        }
        bool lineFree = false;
        std::this_thread::sleep_for(std::chrono::milliseconds(intensClients));
        for (auto it = lines.begin(); it != lines.end(); it++) {
            if ((*it)->size() < line_len) {
                (*it)->push(getClient());
                lineFree = true;
                break;
            }
        }
        if (!lineFree) {
            if (activeLines < numCass) {
                activeLines++;
                auto newLine = new std::queue <Client*>;
                newLine->push(getClient());
                lines.push_back(newLine);
                casses.push_back(new std::thread(&Market::serveLine, this, newLine));
            }
            else {
                unservedClients++;
            }
        }
    }
    finished = true;
}