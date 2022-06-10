// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TEST1, 1) {
    int cassNum = 5;
    int maxNumClients = 120;
    int intensClients = 15;
    int servSpeed = 30;
    int averItemNum = 10;
    int maxLen = 5;
    Market m(cassNum, maxNumClients, intensClients, servSpeed, averItemNum, maxLen);
    m.start();
    ASSERT_GT(m.getAverageWaitingTimeForClient(), 300);
}

TEST(TEST2, 2) {
    int cassNum = 5;
    int maxNumClients = 100;
    int intensClients = 5;
    int servSpeed = 20;
    int averItemNum = 10;
    int maxLen = 2;
    Market m(cassNum, maxNumClients, intensClients, servSpeed, averItemNum, maxLen);
    m.start();
    ASSERT_GT(m.getAmountOfUnservedClients(), 95);
}

TEST(TEST3, 3) {
    int cassNum = 5;
    int maxNumClients = 120;
    int intensClients = 15;
    int servSpeed = 30;
    int averItemNum = 10;
    int maxLen = 5;
    Market m(cassNum, maxNumClients, intensClients, servSpeed, averItemNum, maxLen);
    m.start();
    ASSERT_GT(m.getAmountOfServedClients(), 15);
}
