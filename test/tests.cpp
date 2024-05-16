// Copyright 2024 Kapochkin Mikhail
#include <gtest/gtest.h>
#include "task1.h"
TEST(TestClient, ClientConstruct) {
    Client client(2, 8);
    EXPECT_EQ(client.id, 2);
    EXPECT_EQ(client.cart.size(), 8);
}

TEST(SuperMarketTest, WorkFlowSim) {
    SuperMarket market(4, 10, 10);
    market.SimulateWork(40, 4, 4);
    EXPECT_EQ(market.statistic.rejClients + market.statistic.appClients, 40);
    EXPECT_TRUE(market.statistic.rejClients = 0);
}

TEST(SuperMarketTest, EmptyQueueFormed) {
    SuperMarket market(3, 0, 45);
    market.SimulateWork(100, 25, 4);
    EXPECT_EQ(market.statistic.appClients, 0);
}

TEST(CashMachineTest, CashMachineBusyState) {
    CashRegister cashRegister(100);
    EXPECT_FALSE(cashRegister.IsBusy());
}
