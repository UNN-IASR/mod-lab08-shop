// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"
TEST(Test1, NoRejected) {
    Market m(2, 100, 2, 5, 2);
    m.Work(15);
    EXPECT_EQ(m.rejectedCustomer, 0);
}

TEST(Test2, NoLost) {
    Market m(2, 100, 2, 5, 2);
    m.Work(15);
    EXPECT_EQ(m.acceptedCustomer + m.rejectedCustomer, 15);
}

TEST(Test3, RejectProbability) {
    Market m(3, 500, 5, 3, 5);
    m.Work(20);
    ASSERT_TRUE(m.rejectedCustomer / 20 < 0.5);
}

TEST(Test4, NoQueue) {
    Market m(3, 500, 5, 3, 0);
    m.Work(20);
    EXPECT_EQ(m.acceptedCustomer, 0);
}
