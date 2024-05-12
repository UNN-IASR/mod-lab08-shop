// Copyright 2024 GHA Test Team
#include <gtest/gtest.h>
#include <task.h>

TEST(test1, served) {
	Shop shop(2, 10, 50, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Served() <= 14);
}
TEST(test2, unserved) {
	Shop shop(2, 10, 50, 9, 5);
	shop.Run();
	EXPECT_TRUE(shop.Unserved() <= 36);
}
TEST(test3, averageDownCash) {
	Shop shop(1, 20, 50, 5, 1);
	shop.Run();
	EXPECT_EQ(shop.AverageDownCash(), 0);
}
TEST(test4, averageWorkCash) {
	Shop shop(2, 10, 10, 9, 50);
	shop.Run();
	EXPECT_TRUE(shop.AverageWorkCash() <
		(shop.AverageProd() * shop.SpeedProc()));
}
TEST(test5, failureProbability) {
	Shop shop(3, 5, 3, 10, 9);
	shop.Run();
	double failureProbability = static_cast<double>(shop.Unserved()) /
		shop.MaxCustom();
	EXPECT_TRUE(failureProbability < 0.5);
}
