#include "task.h"

Buyer::Buyer(std::vector<int> _check, int _number) {
  check = _check;
  number = _number;
}

Supermarket::Supermarket(int _cashRegisterAmount, int _flowRate, int _processingSpeed, int _averageNumberOfProducts, int _maxQueueLength, int _maxBuyers) {
  cashRegisterAmount = _cashRegisterAmount;
  flowRate = _flowRate;
  processingSpeed = _processingSpeed;
  averageNumberOfProducts = _averageNumberOfProducts;
  maxQueueLength = _maxQueueLength;
  maxBuyers = _maxBuyers;
}

Buyer* Supermarket::createBuyer(int number) {
  int countProducts = std::rand() % (averageNumberOfProducts + 5) + (averageNumberOfProducts - 5);
  std::vector<int> check(countProducts);
  for (int i = 0; i < countProducts; i++) {
    check[i] = std::rand() % maxPrice + 1;
  }
  std::cout << "new buyer" << '\n';
  return new Buyer(check, number);
}

void Supermarket::run() {
  serveSupermarket();
  for (auto currQueue : threads) {
    currQueue->join();
  }
}

void Supermarket::serveBuyer(Buyer* _buyer, int _number) {
  for (auto i = 0; i < _buyer->check.size(); i++) {
    int time = std::rand() % processingSpeed;
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    std::unique_lock<std::mutex> unqLock(myMutex);
    std::cout << "Cash register " << std::this_thread::get_id() << ": buyer " << _number << " gets product " << i + 1 << '\n';
    unqLock.unlock();
    serviceTime += time;
    averageCashRegisterWorkingTime += processingSpeed / workingCashRegisterCount;
    averageCashRegisterNotWorkingTime += processingSpeed / (cashRegisterAmount - workingCashRegisterCount);
  }
  servedCustomers++;

}

void Supermarket::serveQueue(std::queue <Buyer*>* _buyers) {
  //int number = 1;
  auto buyer = _buyers->front();
  _buyers->pop();
  serveBuyer(buyer, buyer->number);

  while (!buyerQueue.empty()) {
    std::unique_lock<std::mutex> unqLock(myMutex);
    _buyers->push(buyerQueue.front());
    buyerQueue.pop();
    unqLock.unlock();

    auto buyer = _buyers->front();
    _buyers->pop();
    serveBuyer(buyer, buyer->number);
  }
}

void Supermarket::serveSupermarket() {
  for (int i = 0; i < maxBuyers; i++) {
    countBuyers++;
    if (workingCashRegisterCount <= cashRegisterAmount) {
      auto ptrQ = new std::queue<Buyer*>;
      ptrQ->push(createBuyer(countBuyers));
      cashRegisterQueues.push_back(ptrQ);
      threads.push_back(new std::thread(&Supermarket::serveQueue, this, ptrQ));
      workingCashRegisterCount++;
    }

    else if (buyerQueue.size() < maxQueueLength) {
      buyerQueue.push(createBuyer(countBuyers));
    }

    averageQueueLength += buyerQueue.size();

  }
  averageQueueLength = averageQueueLength / countBuyers;
  averageCashRegisterWorkingTime = averageCashRegisterWorkingTime / countBuyers;
  averageCashRegisterNotWorkingTime = averageCashRegisterNotWorkingTime / countBuyers;

  serviceTime = serviceTime / countBuyers;
}

int Supermarket::getServedCustomers() {
  return servedCustomers;
}
int Supermarket::getNotServedCustomers() {
  return countBuyers - servedCustomers;
}
double Supermarket::getAverageQueueLength() {
  return averageQueueLength;
}
double Supermarket::getServiceTime() {
  return serviceTime;
}
double Supermarket::getAverageCashRegisterWorkingTime() {
  return averageCashRegisterWorkingTime;
}
double Supermarket::getAverageCashRegisterNotWorkingTime() {
  return averageCashRegisterNotWorkingTime;
}
double Supermarket::refuseProbability() {
  return (refuseCount / countBuyers);


  double p = static_cast<double>(flowRate) / (processingSpeed);

  double P0 = 1.0;
  for (int i = 1; i <= cashRegisterAmount; i++) {
    P0 = P0 + pow(p, i) / factorial(i);
  }

  for (int i = cashRegisterAmount + 1; i < cashRegisterAmount + maxQueueLength; i++) {
    P0 = P0 + pow(p, i) / (factorial(cashRegisterAmount) * pow(cashRegisterAmount, i - cashRegisterAmount));
  }

  P0 = 1.0 / P0;

  double Prej = P0 * pow(p, (cashRegisterAmount + maxQueueLength)) / static_cast<double>(pow(cashRegisterAmount, maxQueueLength) * factorial(cashRegisterAmount));
  return Prej;
}
double Supermarket::relativeBandwidth() {
  return 1 - refuseProbability();
}
double Supermarket::absoluteBandwidth() {
  return relativeBandwidth() * processingSpeed;
}

double Supermarket::factorial(double i)
{
  if (i == 0) return 1;
  else return i * factorial(i - 1);
}