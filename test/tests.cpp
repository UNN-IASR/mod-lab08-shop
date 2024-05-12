#include <gtest/gtest.h>
#include "../include/task.h"

TEST(test1, p1) {
    Tsum cassa(1,1000,1,1,1);
    cassa.Execute(2);
    EXPECT_EQ(1, cassa.UnServicedCustomers);
}

TEST(test2, p2) {
    Tsum cassa(1,1000,1,1,1);
    cassa.Execute(2);
    EXPECT_EQ(1, cassa.ServicedCustomers);
}

TEST(test3, p3) {
    Tsum cassa(6,80,100,6,10);
    cassa.Execute(200);
    EXPECT_EQ(16.8, cassa.ServicedCustomers/200*80);
}