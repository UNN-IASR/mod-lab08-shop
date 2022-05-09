  // Copyright 2022 UNN-IASR
#include "market.h"
#define rand_r rand
Buyer::Buyer(std::vector<int> _check,
int _timeInQueue, int _timeOnCashBox) {
check = _check;
timeInQueue = _timeInQueue;
timeOnCashBox = _timeOnCashBox;
}
Supermarket::Supermarket(int _buyerquesesnum,
int _buyersIntensity,
int _procSpeedProduct, int _averageProducts,
int _maxBuyers, int _queueLen) :
buyerquesesnum(_buyerquesesnum),
buyersIntensity(_buyersIntensity),
procSpeedProduct(_procSpeedProduct),
averageProducts(_averageProducts),
maxBuyers(_maxBuyers), queueLen(_queueLen),
servedBuyers(0), notServedBuyers(0),
quesescol(0), averagebuyersOnQueue(0.0),
timeCashBoxStop(0),
onCashBox(0.0), inQueue(0.0),
timeCashBoxWork(0.0) {}
Buyer* Supermarket::createBuyer() {
int count = std::rand_r() % (2 * averageProducts) + 1;
std::vector<int> check(count);
for (int i = 0; i < count; i++) {
check[i] = std::rand_r() % 100 + 1;
}
return new Buyer(check, 0, count * procSpeedProduct);
}
void Supermarket::run() {
serveSupermarket();
for (auto currQueue : threads) {
currQueue->join();
}
}
void Supermarket::serveBuyer(Buyer* _buyer, int _number) {
for (auto i = 0; i < _buyer->check.size(); i++) {
std::this_thread::sleep_for(std::chrono::milliseconds(procSpeedProduct));
std::unique_lock<std::mutex> unqLock(myMutex);
std::cout << "Cashbox " << std::this_thread::get_id()
<< ": buyer " << _number << " gets product " << i + 1 << '\n';
unqLock.unlock();
}
}
void Supermarket::serveQueue(std::queue <Buyer*>*_buyers) {
int number = 1;
while (!_buyers->empty()) {
auto buyer = _buyers->front();
_buyers->pop();
serveBuyer(buyer, number);
number++;
}
delete _buyers;
}
void Supermarket::serveSupermarket() {
int* timestop = new int[buyerquesesnum];
for (int i = 0; i < buyerquesesnum; i++) {
timestop[i] = 0;
}
for (int i = 0; i < maxBuyers; i++) {
bool findQueue = false;
for (auto it = buyerQueues.begin(); it != buyerQueues.end(); it++) {
std::this_thread::sleep_for
(std::chrono::milliseconds(1000/buyersIntensity));
if ((*it) == nullptr) {
buyerQueues.erase(it);
continue;
}
if ((*it)->size() < queueLen) {
if ((*it)->size() > 0) {
int timeInQueue = 0;
timeInQueue = (*it)->back()->timeInQueue
+ (*it)->back()->timeOnCashBox;
(*it)->push(createBuyer());
(*it)->back()->timeInQueue = timeInQueue;
} else {
(*it)->push(createBuyer());
}
inQueue += (*it)->back()->timeInQueue;
onCashBox += (*it)->back()->timeOnCashBox;
servedBuyers++;
findQueue = true;
break;
}
}
if (!findQueue && buyerQueues.size() < buyerquesesnum) {
servedBuyers++;
quesescol++;
auto ptrQ = new std::queue <Buyer*>;
ptrQ->push(createBuyer());
buyerQueues.push_back(ptrQ);
inQueue += ptrQ->back()->timeInQueue;
onCashBox += ptrQ->back()->timeOnCashBox;
threads.push_back(new std::thread(&Supermarket::serveQueue,
this, ptrQ));
}
int averagebuyers = 0;
for (int i = 0; i < buyerQueues.size(); i++) {
averagebuyers += buyerQueues[i]->size();
}
averagebuyersOnQueue = static_cast<double>(averagebuyers)
/ static_cast<double>(quesescol);
for (int i = 0; i < buyerquesesnum; i++) {
if (i < buyerQueues.size()) {
if (buyerQueues[i]->size() == 0) {
timestop[i] += (1000 / buyersIntensity);
}
} else {
timestop[i] += (1000 / buyersIntensity);
}
}
}
for (int i = 0; i < buyerquesesnum; i++) {
timeCashBoxStop += timestop[i];
}
timeCashBoxStop = static_cast<double> (timeCashBoxStop)
/ static_cast<double>(buyerquesesnum);
timeCashBoxWork = static_cast<double> (onCashBox)
/ static_cast<double> (buyerquesesnum);
onCashBox = static_cast<double> (onCashBox) /
static_cast<double> (maxBuyers);
inQueue = static_cast<double> (inQueue)
/ static_cast<double> (maxBuyers);
}
int Supermarket::ServedBuyers() {
return servedBuyers;
}
int Supermarket::NotServedBuyers() {
return maxBuyers - servedBuyers;
}
double Supermarket::AverageBuyersOnQueue() {
return averagebuyersOnQueue;
}
double Supermarket::TimeCashBoxStop() {
return timeCashBoxStop;
}
double Supermarket::OnCashBox() {
return onCashBox;
}
double Supermarket::InQueue() {
return inQueue;
}
double Supermarket::TimeCashBoxWork() {
return timeCashBoxWork;
}
double Supermarket::FailureProbability() {
double p = 0.0;
double intensityenter = static_cast<double> (1000.0)
/ static_cast<double> (procSpeedProduct * averageProducts);
p = static_cast<double> (buyersIntensity)
/ static_cast<double> (intensityenter);
double P0 = 0.0;
for (int i = 0; i <= buyerquesesnum * queueLen; i++) {
if (i <= buyerquesesnum) {
P0 += static_cast<double> (std::pow(p, i)) / Factorial(i);
} else {
P0 += static_cast<double> (std::pow(p, i))
/ (Factorial(buyerquesesnum) * std::pow(buyerquesesnum, i));
}
}
P0 = 1 / P0;
double Prej = static_cast<double> (P0)
* (std::pow(p, buyerquesesnum * queueLen) /
(Factorial(buyerquesesnum)
* std::pow(buyerquesesnum, buyerquesesnum * queueLen)));
return Prej;
}
double Supermarket::RelativeThroughput() {
return 1 - FailureProbability();
}
double Supermarket::AbsoluteBandwidth() {
return buyersIntensity * RelativeThroughput();
}
int Supermarket::Factorial(int n) {
if (n == 0) return 1;
if (n == 1) return 1;
return n * Factorial(n - 1);
}
