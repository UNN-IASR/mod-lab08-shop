// Copyright 2024 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

using namespace std;
//test1 Инициализация
TEST(test_first, first) {
	Shop purchase(4, 20, 600, 5, 4);
	ASSERT_TRUE(purchase.buy_count == 5 && purchase.Time == 600 && purchase.count == 4 && purchase.threads.size() > 0);
}
//test2 моедлирования работы
TEST(test_second, second) {
	purchase.Start_working(true);
	ASSERT_TRUE(purchase.buy_time > 3000 && purchase.wait_time > 5000 && purchase.work_time > 11000);
}
//test3 итоговых пропускных способностей многоканальной СВО с ограниченной очередью
TEST(test_third, third) {
	Shop purchase(4, 20, 600, 5, 4);
	ASSERT_TRUE(purchase.A > 0.8 && purchase.Q > 0.4);
}
