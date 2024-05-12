// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(Test1, Test1) {
    int cashNum = 3;
    double clientFlow = 5.0;
    double cashSpeed = 2.0;
    int queueMax = 5;
    int cartSize = 3;

    Store sto = Store(cashNum, cashSpeed, queueMax);
    ClientGenerator cli = ClientGenerator(&sto, cartSize);
    for (int id = 1; id <= 200; id++) {
        cli.Send(id);
        auto delay = std::chrono::milliseconds((int)(1000.0 / clientFlow));
        std::this_thread::sleep_for(delay);
    }
    sto.Close();
    sto.Stats();

    ASSERT_TRUE((double)sto.rejected / 200 < 0.6);
}

TEST(Test2, Test2) {
    int cashNum = 3;
    double clientFlow = 5.0;
    double cashSpeed = 2.0;
    int queueMax = 0;
    int cartSize = 3;

    Store sto = Store(cashNum, cashSpeed, queueMax);
    ClientGenerator cli = ClientGenerator(&sto, cartSize);
    for (int id = 1; id <= 200; id++) {
        cli.Send(id);
        auto delay = std::chrono::milliseconds((int)(1000.0 / clientFlow));
        std::this_thread::sleep_for(delay);
    }
    sto.Close();
    sto.Stats();

    EXPECT_EQ(sto.accepted, 0);
}

TEST(Test3, Test3) {
    int cashNum = 2;
    double clientFlow = 2.0;
    double cashSpeed = 10.0;
    int queueMax = 4;
    int cartSize = 5;

    Store sto = Store(cashNum, cashSpeed, queueMax);
    ClientGenerator cli = ClientGenerator(&sto, cartSize);
    for (int id = 1; id <= 200; id++) {
        cli.Send(id);
        auto delay = std::chrono::milliseconds((int)(1000.0 / clientFlow));
        std::this_thread::sleep_for(delay);
    }
    sto.Close();
    sto.Stats();

    EXPECT_EQ(sto.rejected, 0);
}

TEST(Test4, Test4) {
    int cashNum = 2;
    double clientFlow = 2.0;
    double cashSpeed = 10.0;
    int queueMax = 4;
    int cartSize = 5;

    Store sto = Store(cashNum, cashSpeed, queueMax);
    ClientGenerator cli = ClientGenerator(&sto, cartSize);
    for (int id = 1; id <= 200; id++) {
        cli.Send(id);
        auto delay = std::chrono::milliseconds((int)(1000.0 / clientFlow));
        std::this_thread::sleep_for(delay);
    }
    sto.Close();
    sto.Stats();

    EXPECT_EQ(sto.total, 200);
}