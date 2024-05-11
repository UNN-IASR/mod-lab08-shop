// Copyright 2021 GHA Test Team

#include <gtest/gtest.h>

#define private public
#include "task.h"

TEST(market, produce_clientAddedToQueue) {
    Market m(1, 1, 1, 1, 1);

    auto start = std::chrono::steady_clock::now();
    m.Produce(1);
    auto stop = std::chrono::steady_clock::now();

    int milliseconds = (stop - start).count() / 1000000;
    EXPECT_EQ(1000, milliseconds);
    EXPECT_EQ(1, m.clientsQueue.size());
}

TEST(market, produce_clientRejected) {
    Market m(1, 1000, 1, 1, 1);

    m.Produce(2);

    EXPECT_EQ(1, m.clientsQueue.size());
    EXPECT_EQ(1, m.RejectedClients);
}

TEST(market, consume_oneClient) {
    Market m(1, 1000, 1, 100, 1);
    m.Produce(1);
    
    auto start = std::chrono::steady_clock::now();
    m.Consume();
    auto stop = std::chrono::steady_clock::now();
    
    int milliseconds = (stop - start).count() / 1000000;
    EXPECT_EQ(100, milliseconds);
    EXPECT_EQ(0, m.clientsQueue.size());
    EXPECT_EQ(1, m.ServedClients);
}


