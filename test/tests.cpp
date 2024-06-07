// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#define private public
#include "../include/task.h"

TEST(shop, produce_clientAddedToQueue) {
    Shop s(1, 1, 1, 1, 1);
    auto start = std::chrono::steady_clock::now();
    s.Produce(1);
    auto stop = std::chrono::steady_clock::now();
    int milliseconds = (stop - start).count() / 1000000;
    EXPECT_EQ(1000, milliseconds);
    EXPECT_EQ(1, s.clientsQueue.size());
}

TEST(shop, produce_clientRejected) {
    Shop s(1, 1000, 1, 1, 1);
    s.Produce(2);
    EXPECT_EQ(1, s.clientsQueue.size());
    EXPECT_EQ(1, s.RejectedClients);
}

TEST(shop, Clients) {
    Shop shop(1, 1000, 1, 1, 1);
    shop.Produce(200);
    int result = shop.RejectedClients + shop.ServedClients;
    EXPECT_EQ(result, 199);
}

TEST(shop, produce_clientServered) {
    Shop shop(1, 1000, 1, 1, 1);
    shop.Produce(100);
    EXPECT_EQ(0, shop.ServedClients);
}

TEST(shop, ComparisonRejected) {
    Shop s1(10, 10, 5, 1000, 8);
    Shop s2(10, 10, 10, 1000, 8);
    s1.Produce(100);
    s2.Produce(100);
    bool flag = true;
    if (s1.RejectedClients > s2.RejectedClients) flag = false;
    EXPECT_EQ(flag, true);
}

TEST(shop, ComparisonServered) {
    Shop s1(10, 10, 5, 1000, 8);
    Shop s2(10, 10, 10, 1000, 8);
    s1.Produce(100);
    s2.Produce(100);
    bool flag = true;
    if (s1.ServedClients < s2.ServedClients) flag = false;
    EXPECT_EQ(flag, true);
}
