#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TestCaseName, Test1) {
    Shop shop(10, 40, 0.5, 9, 6);
    shop.Processing();
    int result = shop.serviced_customers + shop.unservesed_customers;
    EXPECT_EQ(result, 40);
}
TEST(TestCaseName, Test4) {
    Shop shop1(10, 40, 0.5, 9, 6);
    Shop shop2(10, 40, 0.8, 9, 6);
    shop1.Processing();
    shop2.Processing();
    bool flag = true;
    if (shop1.serviced_customers > shop2.serviced_customers) flag = false;
    EXPECT_EQ(flag, true);
}

TEST(TestCaseName, Test6) {
    Shop shop1(10, 40, 0.5, 9, 6);
    shop1.Processing();
    shop1.Data_statistics();
    bool flag = false;
    if (shop1.Pre <= 1) flag = true;
    EXPECT_EQ(flag, true);
}
