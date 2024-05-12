// Copyright 2024
#include "task.h"

int GenRandom(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

static int Factorial(int n) {
    int answer = 1;
    for (int i = 1; i <= n; i++) {
        answer *= i;
    }
    return answer;
}

Shop::Shop(int numCashReg, int flowIntens, int speedProc, int averageProd,
    int maxLength) {
    this->numCashReg = numCashReg;
    this->flowIntens = flowIntens;
    this->speedProc = speedProc;
    this->averageProd = averageProd;
    this->maxLength = maxLength;
}

std::shared_ptr<Shop::Customer> Shop::GenCust() {
    int prodCount = averageProd + GenRandom(1, averageProd);
    std::vector<int> list;
    list.resize(prodCount);
    for (int i = 0; i < prodCount; ++i) {
        list[i] = GenRandom(1, 50);
    }
    return std::make_shared<Customer>(list);
}

void Shop::Run() {
    int sum = 0;
    for (int i = 0; i < maxCustom; ++i) {
        bool remain = false;
        for (auto it = begin(queueB);
            it != end(queueB); ++it) {
            sum += (*it)->size();
        }
        averageLen = (averageLen * i + sum) / double(i + 1);
        for (auto it = begin(queueB);
            it != end(queueB); ++it) {
            std::this_thread::sleep_for(std::chrono::milliseconds(flowIntens));
            if ((*it)->size() < maxLength) {
                (*it)->push(GenCust());
                remain = true;
                break;
            }
        }
        if (!remain && queueB.size() < numCashReg) {
            queueCust q =
                std::make_shared<std::queue<std::shared_ptr<Customer>>>();
            q->push(GenCust());
            queueB.push_back(q);
            threads.push_back(std::thread(&Shop::ServeQ, this, q));
        }
    }
    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

void Shop::ServeCust(Customer& buyer) {
    for (int i = 0; i < buyer.shopList.size(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(speedProc));
        std::unique_lock<std::mutex> lock(mutex);
        averageWorkCash += (speedProc * int(queueB.size())
            / double(numCashReg));
        time += speedProc * int(queueB.size());
        averageDownCash += (speedProc * (numCashReg - int(queueB.size()))
            / double(numCashReg));
        lock.unlock();
    }
}

void Shop::ServeQ(queueCust queue) {
    while (!queue->empty()) {
        Customer curr = *queue->front();
        queue->pop();
        ServeCust(curr);
        averageTime = (averageTime * served + speedProc * curr.shopList.size())
            / double(served + 1.0);
        served++;
    }
}

void Shop::Statistic() {
    std::cout << "Served customers: " <<
        Served() << std::endl;
    std::cout << "Unserved customers: " <<
        Unserved() << std::endl;
    std::cout << "Average queue length: " <<
        AverageLen() << std::endl;
    std::cout << "Average Buyer Expected Time: " <<
        AverageTime() << std::endl;
    std::cout << "Average CashBox WorkTime: " <<
        AverageWorkCash() << std::endl;
    std::cout << "Average Cash Box Idle Time: " <<
        AverageDownCash() << std::endl;

    std::cout << "\n\tActual values" << std::endl;
    double failureProbability = static_cast<double>(Unserved()) / maxCustom;
    std::cout << "Failure probability: " << failureProbability << std::endl;
    double relativeThroughput = static_cast<double>(Served()) / maxCustom;
    std::cout << "Relative throughput: " << relativeThroughput << std::endl;
    double absoluteThroughput = Served() / (time * 0.001);
    std::cout << "Absolute throughput: " << absoluteThroughput << std::endl;

    std::cout << "\n\tTheoretical values" << std::endl;
    double r = flowIntens * averageProd / speedProc;
    double Po = 0;
    for (int i = 0; i <= numCashReg; i++)
    {
        Po += (pow(r, i) / Factorial(i));
    }
    Po = 1 / Po;
    failureProbability = pow(r, numCashReg) / Factorial(numCashReg) * Po;
    std::cout << "Failure probability: " << failureProbability << std::endl;
    relativeThroughput = 1 - failureProbability;
    std::cout << "Relative throughput: " << relativeThroughput << std::endl;
    absoluteThroughput = flowIntens * relativeThroughput;
    std::cout << "Absolute throughput: " << absoluteThroughput << std::endl;
}

int Shop::NumCashReg() {
    return numCashReg;
}
int Shop::FlowIntens() {
    return flowIntens;
}
int Shop::SpeedProc() {
    return speedProc;
}
int Shop::AverageProd() {
    return averageProd;
}
int Shop::MaxLength() {
    return maxLength;
}
int Shop::MaxCustom() {
    return maxCustom;
}
int Shop::Served() {
    return served;
}
int Shop::Unserved() {
    return maxCustom - served;
}
double Shop::AverageLen() {
    return averageLen;
}
double Shop::AverageTime() {
    return averageTime * 0.001;
}
double Shop::AverageWorkCash() {
    return averageWorkCash * 0.001;
}
double Shop::AverageDownCash() {
    return averageDownCash * 0.001;
}
