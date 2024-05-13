// Copyright 2024 GHA Test Team

#include <gtest/gtest.h>
#include "../include/task.h"

TEST(ClientTest, ClientCreation) {
    Client client(1, 5);
    EXPECT_EQ(client.id, 1);
    EXPECT_EQ(client.cart.size(), 5);
}

TEST(CashRegisterTest, CashRegisterCreation) {
    CashRegister cashRegister(100);
    EXPECT_FALSE(cashRegister.IsBusy());
}

TEST(ShopTest, Simulation) {
    Shop shop(3, 6, 45);
    shop.Simulate(100, 25, 4);
    EXPECT_EQ(shop.statistics.rejectedClients + shop.statistics.servedClients, 100);
    EXPECT_TRUE(shop.statistics.rejectedClients > 0);
}

TEST(ShopTest, NoQueue) {
    Shop shop(3, 0, 45);
    shop.Simulate(100, 25, 4)
    EXPECT_EQ(shop.statistics.servedClients, 0);
}