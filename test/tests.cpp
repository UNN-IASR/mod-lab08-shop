// Copyright 2024 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"


TEST(test1, T1) {
	Shop shop(2, 10, 50, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Served() <= 10);
}
TEST(test2, T2) {
	Shop shop(2, 10, 10, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Unserved() <= 15);
}
TEST(test3, T3) {
	Shop shop(1, 20, 50, 5, 1);
	shop.Run();
	EXPECT_TRUE(shop.Av_lenght() <= 30);
}
TEST(test4, T4) {
	Shop shop(2, 10, 10, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Av_time() <= 1);
}
TEST(test5, T5) {
	Shop shop(2, 10, 10, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Av_wait_time() <= 1);
}
TEST(test6, T6) {
	Shop shop(2, 10, 10, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Av_work_time() >= 1);
}
