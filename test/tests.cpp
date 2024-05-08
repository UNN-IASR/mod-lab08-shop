// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(test1, nobodyRejected) {
    Store s { 2, 2, 100, 5, 2, 15 };
    s.simulate();
    EXPECT_EQ(s.rejectedCustomer, 0);
}

TEST(test2, nobodyLost) {
    Store s { 2, 2, 100, 5, 2, 15 };
    s.simulate();
    EXPECT_EQ(s.rejectedCustomer + s.acceptedCustomer, 15);
}

TEST(test3, rejectProbability) {
    Store s { 3, 5, 500, 3, 5, 20 };
    s.simulate();
    ASSERT_TRUE((double)s.rejectedCustomer / 20 < 0.5);
}

TEST(test4, noQueue) {
    Store s { 3, 5, 500, 3, 0, 20 };
    s.simulate();
    EXPECT_EQ(s.acceptedCustomer, 0);
}