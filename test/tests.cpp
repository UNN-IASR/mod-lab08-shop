// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(test1, servedCustomers) {
	Shop shop(1, 50, 40, 1, 4, 50);
	shop.InitShop();
	int result = shop.servedCustomers;
	EXPECT_EQ(result, 50);
}

TEST(test2, unservedCustomers) {
	Shop shop(1, 50, 40, 1, 4, 50);
	shop.InitShop();
	int result = shop.unservedCustomers;
	EXPECT_EQ(result, 0);
}

TEST(test3, avgQueueLen) {
	Shop shop(1, 50, 50, 1, 4, 50);
	shop.InitShop();
	int result = shop.spareTimeCount;
	EXPECT_EQ(result, 0);
}

TEST(test4, allRequestsServed) {
	Shop shop(2, 50, 40, 5, 4, 20);
	shop.InitShop();
	int result = shop.servedCustomers + shop.unservedCustomers;
	EXPECT_EQ(result, 20);
}
