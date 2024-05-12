// Copyright 2024 Yuriy Nazarov
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TestCaseName, Test1) {
    Shop shop(5, 20, 0.1, 10, 5);
    shop.StartWork();
    int result = shop.countServedClients + shop.countUnservedClients;
    EXPECT_EQ(result, 20);
}

TEST(TestCaseName, Test2) {
    Shop shop(5, 20, 0.1, 10, 5);
    shop.StartWork();
    EXPECT_EQ(shop.countServedClients, 12);
}

TEST(TestCaseName, Test3) {
    Shop shop1(5, 20, 0.1, 10, 5);
    Shop shop2(5, 20, 0.3, 10, 5);
    shop1.StartWork();
    shop2.StartWork();
    bool better = false;
    if (shop1.countServedClients < shop2.countServedClients) better = true;
    EXPECT_EQ(better, true);
}
TEST(TestCaseName, Test4) {
    Shop shop1(5, 20, 0.1, 10, 5);
    Shop shop2(5, 20, 0.3, 10, 5);
    shop1.StartWork();
    shop2.StartWork();
    bool better = true;
    if (shop1.countServedClients > shop2.countServedClients) better = false;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, Test5) {
    Shop shop1(5, 20, 0.1, 10, 5);
    Shop shop2(10, 20, 0.1, 10, 5);
    shop1.StartWork();   
    shop2.StartWork();
    bool better = false;
    if (shop1.avgQueueLength > shop2.avgQueueLength) better = true;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, Test6) {
    Shop shop1(2, 30, 0.1, 5, 10);
    shop1.StartWork();
    shop1.CalculateStatistics();
    bool t = false;
    if (shop1.pPn <= 1) t = true;
    EXPECT_EQ(t, false);
}