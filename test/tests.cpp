// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../src/task.cpp"
/* #include "task1.h"
#include "task2.h"
#include "task3.h" */

TEST(sample_test_case, sample_test)
{
    Buyer Bob(10);
    Buyer Bob1(10);
    EXPECT_EQ(Bob1.Get_ID(),2);
}