// Copyright 2024 Chernyadeva Anna
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(Test, PropusknayaSposobnost) {
    Shop shop(5, 1, 0.1, 10, 5);
    shop.Start();
    EXPECT_EQ(50, shop.servedCustomers + shop.unservedCustomers);
}

TEST(Test, ObsluzivanieNaCasse) {
    Shop shop(1, 1, 1, 1, 1);
    shop.Start();
    EXPECT_EQ(50, shop.servedCustomers);
    EXPECT_EQ(0, shop.unservedCustomers);
}

TEST(Test, VremyaRaboti) {
    Shop shop(1, 1, 1, 1, 1);
    shop.Start();
    bool f = false;
    if (shop.allTime < 6000) f = true;
    EXPECT_EQ(f, true);
}
