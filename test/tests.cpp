// Copyright 2021 GHA Test Team
#include "../include/task.h"
#include <gtest/gtest.h>

TEST(TestCaseName, Test1) {
    Store store(5, 50, 0.1, 10, 10);
    store.Run();
    EXPECT_EQ(store.serviced + store.notServiced, 50);
}

TEST(TestCaseName, Test2) {
    Store store1(5, 50, 0.1, 10, 10);
    store1.Run();
    bool equaled = true;
    if (store1.notServiced < 25) 
        equaled = false;
    EXPECT_EQ(equaled,true);
}

TEST(TestCaseName, Test3) {
    Store store1(5, 50, 0.1, 10, 10);
    store1.Run();
    bool equaled = true;
    if (store1.serviced + store1.notServiced + store2.serviced + store2.notServiced != 100) 
        equaled = false;
    EXPECT_EQ(equaled,true);
    EXPECT_EQ(store1.notServiced, 32);
}

TEST(TestCaseName, Test4) {
    Store store1(5, 10, 0.1, 10, 10);
    store1.Run();
    EXPECT_EQ(store1.notServiced, 0);
}

TEST(TestCaseName, Test5) {
    Store store1(5, 10, 0.1, 10, 10);
    store1.Run();
    EXPECT_EQ(store1.queue.size(), 0);
}
