#include "task.h"

Shop::Shop(int cash, double shoppersIntensity,
           double productCashTime, double avgProducts, 
           int maxQueueLength, int customers){
    this->cash = cash;
    this->shoppersIntensity = shoppersIntensity;
    this->productCashTime = productCashTime;
    this->avgProducts = avgProducts;
    this->maxQueueLength = maxQueueLength;
    this->customers = customers;
    this->customersVisited = 0;
    this->isBusy = std::vector<bool>(cash, false);
    this->queue = std::deque<int>();
    this->cashiers = std::vector<std::thread>(cash);
    this->statistic = Stats();
}

Shop::~Shop()
{
    for (int i = 0; i < this->cash; i ++){
        if (this->cashiers[i].joinable())
            this->cashiers[i].join();
    }
}

void Shop::run(){
    std::mutex rm;
    std::thread incoming (&Shop::incomingCustomers, this);
    int prods = 0;
    while (this->customersVisited < this->customers){
        if (!this->queue.empty()){
            for (int i = 0; i < isBusy.size(); i++){
                if (!isBusy[i]) {
                    rm.lock();
                    isBusy[i] = true;
                    prods = this->queue.front();
                    this->queue.pop_front();
                    rm.unlock();
                    cashiers[i].join();
                    cashiers[i] = std::thread (&Shop::productProcessing, this, prods, i);
                    break;
                }
            }
        }
    }
    for (int i = 0; i < cashiers.size(); i++){
        cashiers[i].join();
    }
    std::cout << "Dying" << std::endl;
}

void Shop::incomingCustomers(){
    std::mutex im;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution dist_prods{this->avgProducts, sqrt(sqrt(this->avgProducts))};
    std::normal_distribution dist_custs{this->shoppersIntensity, sqrt(sqrt(this->shoppersIntensity))};
    for (this->customersVisited = 0; 
         this->customersVisited < this->customers;
         this->customersVisited++){
        std::this_thread::sleep_for(std::chrono::milliseconds((int)round(1000/abs(dist_custs(gen)))));
        int prods = abs(dist_prods(gen));
        if (this->queue.size() < this->maxQueueLength){
            im.lock();
            this->queue.emplace_back(prods);
            im.unlock();
            this->statistic.served ++;
        }
        else {
            this->statistic.rejected ++;
        }
    }
}

void Shop::productProcessing(int prodNum, int id){
    std::mutex pm;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)round(prodNum * productCashTime * 1000)));
    pm.lock();
    this->isBusy[id] = false;
    pm.unlock();
}

Stats::Stats()
{
    this->served = 0;
    this->rejected = 0;
    this->avgQueueLength = 0;
    this->avgServeTime = 0;
    this->avgCashWorkTime = 0;
    this->avgCashWaitTime = 0;
}
