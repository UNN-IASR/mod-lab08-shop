// Copyright 2022 Open Cascade team
// Author: Semen Smirnov

// My includes
#include <task.h>

int main() {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 20;
  int aSellersSpeed = 50;
  int aAverageNumber = 5;
  int aCashBoxCount = 1;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
             aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
             aAverageNumber, aCashBoxCount);

  aShop.run();

  std::cout << "Served Customers Count: " <<
    aShop.getServedCustomersCount() << std::endl;
  std::cout << "Unserved Customers Count: " <<
    aMaxBuyers - aShop.getServedCustomersCount() << std::endl;
  std::cout << "Average queue length: " <<
                aShop.getAverageQueueLength() << std::endl;
  std::cout << "Average Buyer Expected Time: " <<
                aShop.getAverageBuyerExpectedTime()* 0.001 << std::endl;
  std::cout << "Average CashBox WorkTime: " <<
                aShop.getAverageCashBoxWorkTime() * 0.001 << std::endl;
  std::cout << "Average Cash Box Idle Time: " <<
               aShop.getAverageCashBoxIdleTime() * 0.001 << std::endl;
}
