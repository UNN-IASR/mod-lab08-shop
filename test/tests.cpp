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
}

TEST(shop, RejectedClients) {
    Shop s(1, 1000, 1, 1, 1);

    s.Produce(2);

    EXPECT_EQ(1, s.RejectedClients);
}

TEST(shop, consume_oneClient) {
    Shop s(1, 1000, 1, 100, 1);
    s.Produce(1);

    auto start = std::chrono::steady_clock::now();
    s.Consume();
    auto stop = std::chrono::steady_clock::now();

    int milliseconds = (stop - start).count() / 1000000;
    EXPECT_EQ(100, milliseconds);
    EXPECT_EQ(0, s.clientsQueue.size());
    EXPECT_EQ(1, s.ServedClients);
}

TEST(shop, RejectedClients_2) {
    Shop s1(5, 20, 1, 100, 5);
    Shop s2(5, 20, 5, 100, 5);
    
    s1.Produce(100);
    s2.Produce(100);
    
    bool better = true;
    if (s1.RejectedClients > s2.RejectedClients) better = false;
    EXPECT_EQ(better, true);
}

TEST(shop, RejectedClients_3) {
    Shop s(5, 20, 1, 100, 5);
    
    s.Produce(100);
    
    bool better = true;
    if (s.ServedClients > s.RejectedClients) better = false;
    EXPECT_EQ(better, true);
}
