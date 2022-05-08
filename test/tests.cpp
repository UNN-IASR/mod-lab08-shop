// Copyright 2022 Open Cascade team
// Author: Semen Smirnov

#include <gtest/gtest.h>
#include <task.h>

TEST(TEST1, getServedCustomersCount) {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 0;
  int aSellersSpeed = 200;
  int aAverageNumber = 5;
  int aCashBoxCount = 1;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
    aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
    aAverageNumber, aCashBoxCount);

  aShop.run();

  EXPECT_EQ(aShop.getServedCustomersCount() < aMaxBuyers * 0.1, true);
}

TEST(TEST2, getAverageRestCashBoxTime) {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 0;
  int aSellersSpeed = 200;
  int aAverageNumber = 5;
  int aCashBoxCount = 1;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
    aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
    aAverageNumber, aCashBoxCount);

  aShop.run();

  EXPECT_EQ(aShop.getAverageCashBoxIdleTime() == 0, true);
}


TEST(TEST3, getAverageRestCashBoxTime) {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 0;
  int aSellersSpeed = 50;
  int aAverageNumber = 5;
  int aCashBoxCount = 1;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
    aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
    aAverageNumber, aCashBoxCount);

  aShop.run();

  EXPECT_EQ(aShop.getAverageCashBoxIdleTime() == 0, true);
}

TEST(TEST4, getAverageCashBoxWorkTime) {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 0;
  int aSellersSpeed = 50;
  int aAverageNumber = 5;
  int aCashBoxCount = 1;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
    aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
    aAverageNumber, aCashBoxCount);

  aShop.run();

  int aMinorTime = aSellersSpeed * aShop.getServedCustomersCount();
  int aMajorTime = aSellersSpeed
    * aAverageNumber * aShop.getServedCustomersCount() * 2;

  EXPECT_EQ(aShop.getAverageCashBoxWorkTime() < aMajorTime &&
            aShop.getAverageCashBoxWorkTime() > aMinorTime, true);
}

TEST(TEST5, getAverageBuyerExpectedTime) {
  int aProductsMax = 10;
  int aMaxPrice = 40;
  int aMaxBuyers = 50;
  int aQueueLenght = 1;
  int aFlowBuyersIntensity = 50;
  int aSellersSpeed = 50;
  int aAverageNumber = 5;
  int aCashBoxCount = 50;
  Shop aShop(aProductsMax, aMaxPrice, aMaxBuyers,
    aQueueLenght, aFlowBuyersIntensity, aSellersSpeed,
    aAverageNumber, aCashBoxCount);

  aShop.run();

  EXPECT_EQ(aShop.getAverageBuyerExpectedTime() > aSellersSpeed
    * aAverageNumber &&
            aShop.getAverageBuyerExpectedTime() < aSellersSpeed
    * aAverageNumber * 2 , true);
}
