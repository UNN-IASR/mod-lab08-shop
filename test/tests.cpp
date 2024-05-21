// Copyright 2024 Alexandr Kokorin
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TestCase, Test1) {
    Shop shop(5, 25, 0.1, 10, 5);
    shop.StartWork();
    EXPECT_EQ(shop.countServedClients + shop.countUnservedClients, 25);
}

TEST(TestCase, Test2) {
    Shop shop(5, 20, 0.1, 10, 5);
    shop.StartWork();
    EXPECT_EQ(shop.countServedClients, 20);
}

TEST(TestCase, Test3) {
    Shop shop1(5, 20, 0.1, 10, 5);
    Shop shop2(5, 20, 0.3, 10, 5);
    shop1.StartWork();
    shop2.StartWork();
    EXPECT_EQ(shop1.countServedClients < shop2.countServedClients, false);
}

TEST(TestCase, Test4) {
    Shop shop1(5, 20, 0.1, 10, 5);
    Shop shop2(5, 20, 0.3, 10, 5);
    shop1.StartWork();
    shop2.StartWork();
    EXPECT_EQ(shop1.countServedClients > shop2.countServedClients, false);
}

TEST(TestCase, Test5) {
    Shop shop1(6, 20, 0.1, 10, 5);
    Shop shop2(11, 20, 0.1, 10, 5);
    shop1.StartWork();
    shop2.StartWork();
    EXPECT_EQ(shop1.queueLength > shop2.queueLength, false);
}
