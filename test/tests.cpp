// Copyright 2024 alenkaLo
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TestCaseName, Test1) {
    Shop shop(5, 20, 0,1, 10, 5);
    shop.Work();
    int result = shop.servedСustomers + shop.unservedCustomers;
    EXPECT_EQ(result, numberRequests);
}

TEST(TestCaseName, Test2) {
    Shop shop(5, 20, 0,1, 10, 5);
    shop.Work();
    EXPECT_EQ(shop.servedСustomers, numberRequests);
}

TEST(TestCaseName, Test3) {
    Shop shop1(5, 20, 0,1, 10, 5);
    Shop shop2(5, 20, 0,3, 10, 5);
    shop1.Work();
    shop2.Work();
    bool better = false;
    if (shop1.servedСustomers < shop2.servedСustomers) better = true;
    EXPECT_EQ(better, true);
}
TEST(TestCaseName, Test4) {
    Shop shop1(5, 20, 0,1, 10, 5);
    Shop shop2(5, 20, 0,3, 10, 5);
    shop1.Work();
    shop2.Work();
    bool better = true
    if (shop1.servedСustomers > shop2.servedСustomers) better = false;
    EXPECT_EQ(better, false);
}

TEST(TestCaseName, Test5) {
    Shop shop1(5, 20, 0,1, 10, 5);
    Shop shop2(10, 20, 0,1, 10, 5);
    shop1.Work();   
    shop2.Work();
    bool better = false;
    if (shop1.avgQueue > shop2.avgQueue) better = true;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, pProbabilityAdequacy) {
    Shop shop1(2, 30, 0.1, 5, 10);
    shop1.Work();
    shop1.Statistics();
    bool t = false;
    if (shop1.pFailureProbability <= 1) t = true;
    EXPECT_EQ(t, true);
}
