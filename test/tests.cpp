// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"


TEST(task1, test1) {
	Shop shop(2, 30, 200, 5, 4);
	shop.Simulation();
	int result = shop.completedClients + shop.rejectedClients;
	EXPECT_EQ(30, result);
}

TEST(task2, test2) {
	Shop shop(4, 60, 200, 5, 8);
	shop.Simulation();
	int result = shop.rejectedClients;
	EXPECT_EQ(0, result);
}

TEST(task3, test3) {
	Shop shop(2, 60, 250, 5, 2);
	shop.Simulation();
	int result = 0;
	if (shop.rejectedClients > 0)
		result = 1;
	EXPECT_EQ(1, result);
}
