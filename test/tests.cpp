// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../src/task.cpp"
/* #include "task1.h"
#include "task2.h"
#include "task3.h" */

TEST(sample_test_case, test1)
{
    Buyer Bob(10);
    Buyer Bob1(10);
    EXPECT_EQ(Bob1.Get_ID(),2);
}

TEST(sample_test_case, test2)
{
    Buyer Bob(10);
    int cap = Bob.Get_size_Cart();
    EXPECT_TRUE(cap >=5 && cap <= 15);
}

TEST(sample_test_case, test3)
{
    Shop shop(1,15,1);
    Buyer Bob(10);
    shop.Processing(Bob);
    shop.Join();
    EXPECT_EQ(shop.requestCount, 1);
}

TEST(sample_test_case, test4)
{
    Shop shop(1,15,1);
    Buyer Bob(10);
    shop.Processing(Bob);
    shop.Join();
    EXPECT_EQ(shop.processedCount, 1);
}

TEST(sample_test_case, test5)
{
    Buyer Bob(10);
    Buyer Bob1(10);
    Shop shop(1,1,0);
    shop.Processing(Bob);
    shop.Processing(Bob1);
    shop.Join();
    EXPECT_EQ(shop.rejectedCount, 1);
}