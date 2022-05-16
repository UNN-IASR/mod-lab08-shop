// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TEST1, buyer_check) {
    int maxPrice = 40;
    int maxBuyers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 20;
    int buyerIntensity = 20;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    Buyer* result = market.createBuyer();
    ASSERT_TRUE(result->check.size() == 1);
}

TEST(TEST2, checkout_time) {
    int maxPrice = 40;
    int maxBuyers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 50;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_TRUE(market.getAverageIdleTime() == 0 && market.getAverageCheckoutTime() == 200);
}

TEST(TEST3, total_served) {
    int maxPrice = 40;
    int maxBuyers = 2;
    int queueLen = 1;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_TRUE((market.getStats().failed_count == 1) && (market.getStats().processed_count == 1));
}

TEST(TEST4, queue_serving) {
    int maxPrice = 40;
    int maxBuyers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 20;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getCountOfWorkingCashDesks(), 1);
}

TEST(TEST5, wait_time) {
    int maxPrice = 40;
    int maxBuyers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 1;
    int goodsService = 50;
    int buyerIntensity = 10;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getAverageWaitTime(), 125);
}

TEST(TEST6, queue_length) {
    int maxPrice = 40;
    int maxBuyers = 4;
    int queueLen = 4;
    int averageNumberOfItems = 5;
    int goodsService = 20;
    int buyerIntensity = 1;
    int numberOfCashDesks = 1;
    Market market(maxPrice, maxBuyers, queueLen, averageNumberOfItems,
        goodsService, buyerIntensity, numberOfCashDesks);
    market.run();
    ASSERT_EQ(market.getAverageQueueLength(), 4);
}