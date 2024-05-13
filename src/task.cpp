#include "task.h"

Shopper::Shopper(int products){
    this->products = products;
    this->birth = std::chrono::system_clock::now();
}

int Shopper::lifeTime(){
    auto const now = std::chrono::system_clock::now();
    std::time_t newt = std::chrono::system_clock::to_time_t(now);
    std::time_t oldt = std::chrono::system_clock::to_time_t(this->birth);
    return newt - oldt;
}

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
    this->queue = std::deque<Shopper>();
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
    this->statistic.openCash(this->cash);
    std::thread incoming (&Shop::incomingCustomers, this);
    while (this->customersVisited < this->customers){
        if (!this->queue.empty()){
            for (int i = 0; i < isBusy.size(); i++){
                if (!isBusy[i]) {
                    rm.lock();
                    this->statistic.updateWaitTime(i);
                    isBusy[i] = true;
                    rm.unlock();
                    if (cashiers[i].joinable()) cashiers[i].join();
                    std::thread t(&Shop::productProcessing, this, this->queue.front(), i);
                    cashiers[i] = std::move(t);
                    rm.lock();
                    this->queue.pop_front();
                    rm.unlock();
                    break;
                }
            }
        }
        this->statistic.sumLengths += queue.size();
        this->statistic.countLenghts ++;
    }
    for (int i = 0; i < cashiers.size(); i++){
        if (cashiers[i].joinable()) cashiers[i].join();
    }
    if (incoming.joinable()) incoming.join();
    this->statistic.calcResults();
}

void Shop::incomingCustomers(){
    std::mutex im;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution dist_prods{this->avgProducts, sqrt(sqrt(this->avgProducts))};
    std::normal_distribution dist_custs{this->shoppersIntensity, sqrt(sqrt(this->shoppersIntensity))};
    int sleepTime = 0;
    int generated = 0;
    for (this->customersVisited = 0; 
         this->customersVisited < this->customers;
         this->customersVisited++){
        generated = abs(dist_custs(gen));
        if (generated == 0) generated = shoppersIntensity;
        sleepTime = (int)round(1000/generated);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
        int prods = abs(dist_prods(gen));
        if (this->queue.size() < this->maxQueueLength){
            im.lock();
            Shopper s(prods);
            this->queue.emplace_back(s);
            im.unlock();
            this->statistic.served ++;
        }
        else {
            this->statistic.rejected ++;
        }
    }
}

void Shop::productProcessing(Shopper s, int id){
    std::mutex pm;
    std::this_thread::sleep_for(std::chrono::milliseconds((int)round(s.products * productCashTime * 1000)));
    pm.lock();
    this->statistic.updateWorkTime(id);
    this->isBusy[id] = false;
    this->statistic.lifeTimes.push_back(s.lifeTime());
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
    this->sumLengths = 0;
    this->countLenghts = 0;
}

void Stats::openCash(int size)
{
    auto const now = std::chrono::system_clock::now();
    this->lastTimePoint = std::vector<std::chrono::time_point<std::chrono::system_clock>> (size, now); 
    this->workTime = std::vector<int> (size, 0);
    this->waitTime = std::vector<int> (size, 0);
}

void Stats::updateWorkTime(int ind)
{
    auto const now = std::chrono::system_clock::now();
    std::time_t newt = std::chrono::system_clock::to_time_t(now);
    std::time_t oldt = std::chrono::system_clock::to_time_t(this->lastTimePoint[ind]);
    this->workTime[ind] += newt - oldt;
    this->lastTimePoint[ind] = now;
}

void Stats::updateWaitTime(int ind)
{
    auto const now = std::chrono::system_clock::now();
    std::time_t newt = std::chrono::system_clock::to_time_t(now);
    std::time_t oldt = std::chrono::system_clock::to_time_t(this->lastTimePoint[ind]);
    this->waitTime[ind] += newt - oldt;
    this->lastTimePoint[ind] = now;
}

void Stats::calcResults()
{
    this->avgQueueLength = this->sumLengths / this->countLenghts;
    double sum = 0;
    for (auto lt : this->lifeTimes){
        sum += lt;
    }
    this->avgServeTime = sum / this->lifeTimes.size();
    sum = 0;
    for (auto wt : this->workTime){
        sum += wt;
    }
    this->avgCashWorkTime = (double)sum / this->workTime.size();
    sum = 0;
    for (auto wt : this->waitTime){
        sum += wt;
    }
    this->avgCashWaitTime = (double)sum / this->waitTime.size();
}
