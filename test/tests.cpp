// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"


TEST(task1, test1) {
	Shop shop(2, 15, 100, 5, 2);
	shop.Work();
	int result = shop.servedClients;
	EXPECT_EQ(15, result);
}

TEST(task2, test2) {
	Shop shop(5, 60, 400, 3, 6);
	shop.Work();
	int result = shop.rejectedClients;
	EXPECT_EQ(0, result);
}

TEST(task3, test3) {
	Shop shop(0, 60, 400, 3, 6);
	shop.Work();
	int result = 0;
	if (shop.servedClients > 0)
		result = 1;
  else 
    return 0;
	EXPECT_EQ(0, result);
}
