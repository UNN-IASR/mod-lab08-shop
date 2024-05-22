// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"
TEST(TestCase, Test1) {
    Shop shop(1,1,1);
    shop.AddNewClient(1, 1);
  	do{
	} while (shop.clientQueue.size() > 0);
    EXPECT_EQ(shop.Buy, 1);
}
