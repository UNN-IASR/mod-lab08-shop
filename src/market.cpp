  // Copyright 2022 UNN-IASR
#include "market.h"
#define rand_r rand
Buyer::Buyer(std::vector<int> _check,
int _timeInQueue, int _timeOnCashBox) {
check = _check;
timeInQueue = _timeInQueue;
timeOnCashBox = _timeOnCashBox;
}
Market::Market(int _buyerquesesnum,
int _buyersIntensity,
int _speedPr, int _averPr,
int _mBuy, int _qLength) :
buyerquesesnum(_buyerquesesnum),
buyersIntensity(_buyersIntensity),
speedPr(_speedPr),
averPr(_averPr),
mBuy(_mBuy), qLength(_qLength),
servedBuyers(0), notServedBuyers(0),
quesescol(0), averBuyOnQ(0.0),
timeCashS(0),
onCash(0.0), inQueue(0.0),
timeCashW(0.0) {}
Buyer* Market::createB() {
int count = std::rand_r() % (2 * averPr) + 1;
std::vector<int> check(count);
for (int i = 0; i < count; i++)  {
check[i] = std::rand_r() % 100 + 1;
}
return new Buyer(check, 0, count * speedPr);
}
void Market::run() {
serveM();
for (auto currQueue : threads) {
currQueue->join();
}
}
void Market::serveB(Buyer* _buyer, int _number) {
for (auto i = 0; i < _buyer->check.size(); i++) {
std::this_thread::sleep_for(std::chrono::milliseconds(speedPr));
std::unique_lock<std::mutex> unqLock(myMutex);
std::cout << "Cashbox " << std::this_thread::get_id()
<< ": buyer " << _number << " gets product " << i + 1 << '\n';
unqLock.unlock();
}
}
void Market::serveQ(std::queue <Buyer*>*_buyers) {
int number = 1;
while (!_buyers->empty()) {
auto buyer = _buyers->front();
_buyers->pop();
serveB(buyer, number);
number++;
}
delete _buyers;
}
void Market::serveM() {
int* timestop = new int[buyerquesesnum];
for (int i = 0; i < buyerquesesnum; i++) {
timestop[i] = 0;
}
for (int i = 0; i < mBuy; i++) {
bool findQueue = false;
for (auto it = buyerQueues.begin(); it != buyerQueues.end(); it++) {
std::this_thread::sleep_for
(std::chrono::milliseconds(1000/buyersIntensity));
if ((*it) == nullptr) {
buyerQueues.erase(it);
continue;
}
if ((*it)->size() < qLength) {
if ((*it)->size() > 0) {
int timeInQueue = 0;
timeInQueue = (*it)->back()->timeInQueue
+ (*it)->back()->timeOnCashBox;
(*it)->push(createB());
(*it)->back()->timeInQueue = timeInQueue;
} else {
(*it)->push(createB());
}
inQueue += (*it)->back()->timeInQueue;
onCash += (*it)->back()->timeOnCashBox;
servedBuyers++;
findQueue = true;
break;
}
}
if (!findQueue && buyerQueues.size() < buyerquesesnum) {
servedBuyers++;
quesescol++;
auto ptrQ = new std::queue <Buyer*>;
ptrQ->push(createB());
buyerQueues.push_back(ptrQ);
inQueue += ptrQ->back()->timeInQueue;
onCash += ptrQ->back()->timeOnCashBox;
threads.push_back(new std::thread(&Market::serveQ,
this, ptrQ));
}
int averagebuyers = 0;
for (int i = 0; i < buyerQueues.size(); i++) {
averagebuyers += buyerQueues[i]->size();
}
averBuyOnQ = static_cast<double>(averagebuyers)
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
timeCashS += timestop[i];
}
timeCashS = static_cast<double> (timeCashS)
/ static_cast<double>(buyerquesesnum);
timeCashW = static_cast<double> (onCash)
/ static_cast<double> (buyerquesesnum);
onCash = static_cast<double> (onCash) /
static_cast<double> (mBuy);
inQueue = static_cast<double> (inQueue)
/ static_cast<double> (mBuy);
}
int Market::ServedB() {
return servedBuyers;
}
int Market::NServedB() {
return mBuy - servedBuyers;
}
double Market::AverBuyersOnQ() {
return averBuyOnQ;
}
double Market::TimeCashS() {
return timeCashS;
}
double Market::OnCash() {
return onCash;
}
double Market::InQueue() {
return inQueue;
}
double Market::TimeCashW() {
return timeCashW;
}
double Market::FailProb() {
double p = 0.0;
double intensityenter = static_cast<double> (1000.0)
/ static_cast<double> (speedPr * averPr);
p = static_cast<double> (buyersIntensity)
/ static_cast<double> (intensityenter);
double P0 = 0.0;
for (int i = 0; i <= buyerquesesnum * qLength; i++) {
if (i <= buyerquesesnum) {
P0 += static_cast<double> (std::pow(p, i)) / factorial(i);
} else {
P0 += static_cast<double> (std::pow(p, i))
/ (factorial(buyerquesesnum) * std::pow(buyerquesesnum, i));
}
}
P0 = 1 / P0;
double Prej = static_cast<double> (P0)
* (std::pow(p, buyerquesesnum * qLength) /
(factorial(buyerquesesnum)
* std::pow(buyerquesesnum, buyerquesesnum * qLength)));
return Prej;
}
double Market::RelThrough() {
return 1 - FailProb();
}
double Market::AbsoluteBand() {
return buyersIntensity * RelThrough();
}
int Market::factorial(int n) {
if (n == 0) return 1;
if (n == 1) return 1;
return n * factorial(n - 1);
}
