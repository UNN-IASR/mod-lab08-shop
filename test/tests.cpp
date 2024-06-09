// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(ShopTest, FailureProbabilityTest) {
    Shop shop(4, 10, 1, 10);
    shop.simulate(10, 2);
    Statistics statistics = shop.printActualStatistics();

    double actual = statistics.probFail;
    EXPECT_TRUE(0 <= actual && actual <= 1);
}

TEST(ShopTest, RelativeThroughputTest) {
    Shop shop(4, 10, 1, 10);

    shop.simulate(10, 2);
    Statistics statistics = shop.printActualStatistics();

    int actual = statistics.throughputRel;
    EXPECT_TRUE(actual <= 1);
}

TEST(ShopTest, ShouldGenerateRandomNumber) {
    int min = -1;
    int max = 1;
    int number = randomInInterval(min, max);
    EXPECT_TRUE(min <= number && number <= max);
}

TEST(ShopTest, ShouldReturnCurrentTime) {
    Shop shop(1, 1, 1, 1);
    int now = shop.getCurrentTime();
    EXPECT_TRUE(now > 0);
}

TEST(ShopTest, SimulationShouldReturnCustomersServed1) {
    int expected = 1;
    Shop shop(1, 1, 1, 1);

    shop.simulate(2, 2);
    shop.printActualStatistics();
    Statistics statistics = shop.printActualStatistics();

    EXPECT_EQ(statistics.customersServed, expected);
}

TEST(ShopTest, SimulationSuccess1) {
    int expectedCustomersServed = 90;
    double expectedProbFail = 0.5;
    double expectedThroughputRel = 0.5;
    Shop shop(0, 10, 1, 10);

    shop.simulate(10, 2);
    Statistics statistics = shop.printActualStatistics();

    EXPECT_EQ(statistics.customersServed, expectedCustomersServed);
    EXPECT_EQ(statistics.probFail, expectedProbFail);
    EXPECT_EQ(statistics.throughputRel, expectedThroughputRel);
}

TEST(ShopTest, SimulationShouldReturnCustomersServed0WhenEmptyQueue) {
    int expected = 2;
    Shop shop(1, 1, 1, 0);

    shop.simulate(2, 2);
    shop.printActualStatistics();
    Statistics statistics = shop.printActualStatistics();

    EXPECT_EQ(statistics.customersServed, expected);
}