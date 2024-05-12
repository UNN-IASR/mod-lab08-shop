//Copyright Annenko Misha 2024

#include "../include/task.h"
#include "gtest/gtest.h"

TEST(Test1, SHOPPER) {
    Shopper shopper = Shopper(5, 1);
    int count = shopper.amount_of_products;
    EXPECT_EQ(count, 5);
}

TEST(Test2, MALL) {
    Shopping_mall mall = Shopping_mall(20);
    bool check = mall.in_service;
    EXPECT_EQ(check, false);
}

TEST(Test3, ClientAdd) {
    Shopping_mall mall1 = Shopping_mall(20);
    Shopper shopper1 = Shopper(4, 2);
    Shopper shopper2 = Shopper(3, 1);
    mall1.Push_back_shopper(shopper1);
    mall1.Push_back_shopper(shopper2);
    int count1 = mall1.shoppers.size();
    EXPECT_EQ(count1, 2);
}

TEST(Test4, Testing_CashChoose) {
    Shopping_mall mall2 = Shopping_mall(20);
    int number = Cash_Choose(&mall2, 2, 4);
    EXPECT_EQ(number, 0);
}

TEST(Test5, Push_back_work) {
    Shopping_mall mall3 = Shopping_mall(20);
    double k = 3;
    while (k < 12) {
        mall3.Push_back_working_time((double)k);
        k++;
    }
    std::vector<double> times = mall3.working_time;
    EXPECT_EQ(times[4], (double)7);
}
