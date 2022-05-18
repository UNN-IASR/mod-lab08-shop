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
class Market{
 private:
std::vector<std::thread*> threads;
std::vector<std::queue<Buyer*>*> buyerQueues;
int averPr;
int mBuy;
int buyersIntensity;
int qLength;
int servedBuyers;
int notServedBuyers;
int speedPr;
int buyerquesesnum;
int quesescol;
double averBuyOnQ;
double timeCashS;
double timeCashW;
double onCash;
double inQueue;
std::mutex myMutex;

 public:
Market(int _buyerquesesnum, int _buyersIntensity,
int _speedPr, int _averPr,
int _mBuy, int _qLength);
Buyer* createB();
void run();
void serveB(Buyer* _buyer, int _number);
void serveQ(std::queue <Buyer*>* _buyers);
void serveM();
int ServedB();
int NServedB();
double AverBuyersOnQ();
double TimeCashS();
double TimeCashW();
double OnCash();
double InQueue();
double FailProb();
double RelThrough();
double AbsoluteBand();
int factorial(int);
};
