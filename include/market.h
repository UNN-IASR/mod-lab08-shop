  // Copyright 2022 UNN-IASR
#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <list>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <random>

class Buyer{
 public:
std::vector<int> check;
int timeInQueue;
int timeOnCashBox;
explicit Buyer(std::vector<int> _check, int _timeInQueue, int _timeOnCashBox);
};
class Supermarket{
 private:
std::vector<std::thread*> threads;
std::vector<std::queue<Buyer*>*> buyerQueues;
int averageProducts;
int maxBuyers;
int buyersIntensity;
int queueLen;
int servedBuyers;
int notServedBuyers;
int procSpeedProduct;
int buyerquesesnum;
int quesescol;
double averagebuyersOnQueue;
double timeCashBoxStop;
double timeCashBoxWork;
double onCashBox;
double inQueue;
std::mutex myMutex;

 public:
Supermarket(int _buyerquesesnum, int _buyersIntensity,
int _procSpeedProduct, int _averageProducts,
int _maxBuyers, int _queueLen);
Buyer* createBuyer();
void run();
void serveBuyer(Buyer* _buyer, int _number);
void serveQueue(std::queue <Buyer*>* _buyers);
void serveSupermarket();
int ServedBuyers();
int NotServedBuyers();
double AverageBuyersOnQueue();
double TimeCashBoxStop();
double TimeCashBoxWork();
double OnCashBox();
double InQueue();
double FailureProbability();
double RelativeThroughput();
double AbsoluteBandwidth();
int Factorial(int);
};
