#include <gtest/gtest.h>
#include "../include/task.h"


TEST(TestCaseName, Test1) {
    Store store(10, 30, 0.2, 8, 6);
    store.work();
    EXPECT_EQ(store.served + store.unserved, 20);
}

TEST(TestCaseName, Test2) {
    Store store(10, 30, 0.2, 8, 6);
    store.work();
    EXPECT_EQ(store.served, 12);
}

TEST(TestCaseName, Test3) {
    Store store1(10, 30, 0.2, 8, 6);
    Store store2(10, 30, 0.4, 8, 6);
    store1.work();
    store2.work();
    bool f = false;
    if (store1.served < store2.served) 
        f = true;
    EXPECT_EQ(f, true);
}
TEST(TestCaseName, Test4) {
    Store store1(10, 30, 0.2, 8, 6);
    Store store2(10, 30, 0.4, 8, 6);
    store1.work();
    store2.work();
    bool f = true;
    if (store1.served > store2.served) 
        f = false;
    EXPECT_EQ(f, true);
}

TEST(TestCaseName, Test5) {
    Store store1(10, 30, 0.2, 8, 6);
    Store store2(10, 30, 0.4, 8, 6);
    store1.work();   
    store2.work();
    bool f = false;
    if (store1.avera > store2.avera) 
        f = true;
    EXPECT_EQ(f, false);
}

TEST(TestCaseName, Test6) {
    Store store(10, 30, 0.2, 8, 6);
    store.work();
    store.stat();
    bool f = false;
    if (store.cancel < 1) 
        f = true;
    EXPECT_EQ(f, true);
}
