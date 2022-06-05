// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"


TEST(task1, test1) {
	Shop shop(1, 10, 100, 1, 1);
	shop.Shopping();
	int result = shop.servedClients;
	EXPECT_EQ(10, result);
}

TEST(task2, test2) {
	Shop shop(8, 92, 300, 5, 8);
	shop.Shopping();
	int result = shop.rejectedClients;
	EXPECT_EQ(0, result);
}

TEST(task3, test3) {
	Shop shop(1, 60, 228, 5, 2);
	shop.Shopping();
	int result = 0;
	if (shop.rejectedClients > 30)
		result = 1;
	EXPECT_EQ(1, result);
}
