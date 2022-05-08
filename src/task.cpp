// Copyright 2022 Open Cascade team
// Author: Semen Smirnov

// My includes
#include <task.h>

Shop::Shop(int theProductsMax, int theMaxPrice, int theMaxBuyers,
           int theQueueLenght, int theFlowBuyersIntensity,
           int theSellersSpeed, int theAverageNumberProducts,
           int theCashBoxCount) :
           myMaxPrice(theMaxPrice),
           myMaxBuyers(theMaxBuyers),
           myQueueLength(theQueueLenght),
           myFlowBuyersIntensity(theFlowBuyersIntensity),
           mySellersSpeed(theSellersSpeed),
           myAverageNumberProducts(theAverageNumberProducts),
           myCashBoxCount(theCashBoxCount),
           myMersenne(myRd()) {}

std::shared_ptr<Shop::Buyer> Shop::createBuyer() {
  std::uniform_int_distribution<> aDistributionProd(1, myAverageNumberProducts);
  int aProductsCount = myAverageNumberProducts + aDistributionProd(myMersenne);

  std::uniform_int_distribution<> aDistributionPrice(1, myMaxPrice);

  std::vector<int> aCheck;
  aCheck.resize(aProductsCount);
  for (size_t i = 0; i < aProductsCount; ++i) {
    aCheck[i] = aDistributionPrice(myMersenne);
  }

  return std::make_shared<Buyer>(aCheck);
}

void Shop::run() {
  serveAllShop();
}

void Shop::serveBuyer(const Buyer& theBuyer,
                      const int theBuyerNumber) {
  for (size_t i = 0; i < theBuyer.myShoppingList.size(); ++i) {
    std::this_thread::sleep_for(std::chrono::milliseconds(mySellersSpeed));

    std::unique_lock<std::mutex> theGuardLock(myMutex);
    myAverageCashBoxTime += (mySellersSpeed *
      static_cast<std::int64_t>(myQueueBuyersVector.size())
      / static_cast<double>(myCashBoxCount));

    myAverageRestCashBoxTime += (mySellersSpeed *
      (myCashBoxCount - static_cast<std::int64_t>(myQueueBuyersVector.size()))
      / static_cast<double>(myCashBoxCount));

#ifdef INFO
    std::cout << "Cashbox = " << std::this_thread::get_id()
              << " Buyer: " << theBuyerNumber << " Product: "
              << i + 1 << std::endl;
#endif
    theGuardLock.unlock();
  }
}

void Shop::serveQueue(SPBuyersQueue theQueueBuyers) {
  int aNumber = 1;

  while (!theQueueBuyers->empty()) {
    Buyer aCurrentBuyer = *theQueueBuyers->front();
    theQueueBuyers->pop();

    serveBuyer(aCurrentBuyer, aNumber);
    aNumber++;

    myAverageQueueTime = (myAverageQueueTime * myBuyerCounter
      + mySellersSpeed * aCurrentBuyer.myShoppingList.size())
      / static_cast<double>(myBuyerCounter + 1.0);

    myBuyerCounter++;
  }

  //TODO: Try to fix this expression to g++ complier.
  //[[maybe_unused]] auto removeIterator =
  //  std::remove(myQueueBuyersVector.begin(), myQueueBuyersVector.end(),
  //    theQueueBuyers);
}

void Shop::serveAllShop() {
  std::int64_t aSizeSum = 0;

  for (size_t i = 0; i < myMaxBuyers; ++i) {
    bool aFindRemainingQueue = false;

    for (auto it = begin(myQueueBuyersVector);
      it != end(myQueueBuyersVector); ++it) {
        aSizeSum += (*it)->size();
    }

    myAverageQueueLength = (myAverageQueueLength * i + aSizeSum)
      / static_cast<double>(i + 1);

    for (auto it = begin(myQueueBuyersVector);
              it != end(myQueueBuyersVector); ++it) {
      std::this_thread::sleep_for(
        std::chrono::milliseconds(myFlowBuyersIntensity));

      if ((*it)->size() < myQueueLength) {
        (*it)->push(createBuyer());
        aFindRemainingQueue = true;
        break;
      }
    }
    if (!aFindRemainingQueue && myQueueBuyersVector.size() < myCashBoxCount) {
      SPBuyersQueue aQueue = std::make_shared<std::queue<SPBuyer>>();

      aQueue->push(createBuyer());

      myQueueBuyersVector.push_back(aQueue);
      myThreads.push_back(std::thread(&Shop::serveQueue, this, aQueue));
    }
  }
  for (size_t i = 0; i < myThreads.size(); ++i) {
    myThreads[i].join();
  }
}

int Shop::getServedCustomersCount() const {
  return myBuyerCounter;
}

int Shop::getUnservedCustomersCount() const {
  return myMaxBuyers - myBuyerCounter;
}

double Shop::getAverageQueueLength() const {
  return myAverageQueueLength;
}

double Shop::getAverageBuyerExpectedTime() const {
  return myAverageQueueTime;
}

double Shop::getAverageCashBoxWorkTime() const {
  return myAverageCashBoxTime;
}

double Shop::getAverageCashBoxIdleTime() const {
  return myAverageRestCashBoxTime;
}
