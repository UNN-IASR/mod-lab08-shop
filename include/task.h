// Copyright 2022 Open Cascade team
// Author: Semen Smirnov

#pragma once

// My includes
#include <iostream>
#include <queue>
#include <memory>
#include <random>
#include <vector>
#include <thread>
#include <mutex>

class Shop {
 private:
  struct Buyer {
    explicit Buyer(const std::vector<int>& theShoppintList)
      : myShoppingList(theShoppintList) {}

    std::vector<int> myShoppingList;
  };

  using SPBuyer = std::shared_ptr<Buyer>;
  using SPBuyersQueue = std::shared_ptr<std::queue<SPBuyer>>;

 public:
  Shop(int theProductsMax, int theMaxPrice, int theMaxBuyers,
       int theQueueLenght, int theFlowBuyersIntensity,
       int theSellersSpeed, int theAverageNumber,
       int theCashBoxCount);

 public:
  void  serveBuyer(const Buyer& theBuyer, const int theBuyerNumber);

  void serveQueue(SPBuyersQueue theQueueBuyers);
  void serveAllShop();

  int getServedCustomersCount() const;
  int getUnservedCustomersCount() const;
  double getAverageQueueLength() const;
  double getAverageBuyerExpectedTime() const;
  double getAverageCashBoxWorkTime() const;
  double getAverageCashBoxIdleTime() const;

  void run();

 private:
  SPBuyer createBuyer();

 private:
  std::vector<std::thread> myThreads;
  std::vector<SPBuyersQueue> myQueueBuyersVector;

  int myMaxPrice;
  int myMaxBuyers;
  int myQueueLength;
  int myCashBoxCount;

  int myFlowBuyersIntensity = 0;
  int mySellersSpeed = 0;
  int myAverageNumberProducts = 0;

  int myBuyerCounter = 0;
  double myAverageQueueLength = 0;
  double myAverageQueueTime = 0;
  double myAverageCashBoxTime = 0;
  double myAverageRestCashBoxTime = 0;

  std::mutex myMutex;
  std::random_device myRd;
  std::mt19937 myMersenne;
};
