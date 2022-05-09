// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include <ctime>
#include "../include/market.h"
TEST(TEST1, ServedBuyers) {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
ASSERT_TRUE(supermarket->ServedBuyers() == 20);
}
TEST(TEST2, NotServedBuyers) {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
ASSERT_TRUE(supermarket->NotServedBuyers() == 0);
}
TEST(TEST3, InQueue) {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
ASSERT_TRUE(static_cast<int>(supermarket->InQueue()) == 0);
}
TEST(TEST4, RelativeThroughput) {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
ASSERT_TRUE(static_cast<int>(supermarket->RelativeThroughput()) == 1);
}

TEST(TEST5, AbsoluteBandwidth) {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
ASSERT_TRUE(static_cast<int>(supermarket->AbsoluteBandwidth()) == 40);
}
