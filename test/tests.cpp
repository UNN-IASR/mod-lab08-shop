// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TEST1, generateBuyer) {
    int maxPrice = 40;
    int maxBayers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 20;
    int buyerIntensity = 20;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    Buyer* result = market.createBuyer();
    ASSERT_TRUE(result->cheak.size() == 1);
}

TEST(TEST2, serveQueueTest) {
    int maxPrice = 40;
    int maxBayers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 20;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getCountOfWorkingCashDesks(), 1);
}

TEST(TEST3, numberOfServicedTest) {
    int maxPrice = 40;
    int maxBayers = 2;
    int queueLen = 1;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_TRUE((market.getStatistics().numberOfFailed == 1) && (market.getStatistics().numberOfProcessed == 1));
}

TEST(TEST4, getAverageQueueLength1) {
    int maxPrice = 40;
    int maxBayers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 1;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getAverageQueueLength(), 4);
}

TEST(TEST6, getAverageTimeOfWaiting) {
    int maxPrice = 40;
    int maxBayers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 50;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getAverageTimeOfWaiting(), 125);
}

TEST(TEST7, getAverageCheckoutTime1) {
    int maxPrice = 40;
    int maxBayers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 50;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBayers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_TRUE(market.getAverageCheckoutTime() == 200 && market.getAverageRestTime() == 0);
}
