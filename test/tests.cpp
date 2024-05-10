#include <gtest/gtest.h>
#include "../include/task.h"


TEST(TestCaseName, Test1) {
    Store store(5, 20, 0.1, 10, 5);
    store.work();
    int result = store.served + store.unserved;
    EXPECT_EQ(result, 20);
}

TEST(TestCaseName, Test2) {
    Store store(5, 20, 0.1, 10, 5);
    store.work();
    EXPECT_EQ(store.served, 12);
}

TEST(TestCaseName, Test3) {
    Store store1(5, 20, 0.1, 10, 5);
    Store store2(5, 20, 0.3, 10, 5);
    store1.work();
    store2.work();
    bool better = false;
    if (store1.served < store2.served) better = true;
    EXPECT_EQ(better, true);
}
TEST(TestCaseName, Test4) {
    Store store1(5, 20, 0.1, 10, 5);
    Store store2(5, 20, 0.3, 10, 5);
    store1.work();
    store2.work();
    bool better = true;
    if (store1.served > store2.served) better = false;
    EXPECT_EQ(better, true);
}

TEST(TestCaseName, Test5) {
    Store store1(5, 20, 0.1, 10, 5);
    Store store2(10, 20, 0.1, 10, 5);
    store1.work();   
    store2.work();
    bool better = false;
    if (store1.length > store2.length) better = true;
    EXPECT_EQ(better, false);
}

TEST(TestCaseName, Test6) {
    Store store(2, 30, 0.1, 5, 10);
    store.work();
    store.stat();
    bool t = false;
    if (store.cancel <= 1) t = true;
    EXPECT_EQ(t, true);
}
