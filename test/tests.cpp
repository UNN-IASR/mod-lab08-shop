#include <gtest/gtest.h>
#include "../include/task.h"


TEST(TestCaseName, Test1) {
    restore restore(12, 40, 0.3, 9, 7);
    restore.job();
    EXPECT_EQ(restore.ready + restore.unready, 30);
}

TEST(TestCaseName, Test2) {
    restore restore1(12, 40, 0.3, 9, 7);
    restore restore2(12, 40, 0.3, 9, 7);
    restore1.job();
    restore2.job();
    bool f = false;
    if (restore1.ready < restore2.ready) 
        f = true;
    EXPECT_EQ(f,true);
}
TEST(TestCaseName, Test3) {
    restore restore1(12, 40, 0.3, 9, 7);
    restore restore2(12, 40, 0.4, 9, 7);
    restore1.job();
    restore2.job();
    bool f = true;
    if (restore1.ready > restore2.ready) 
        f = false;
    EXPECT_EQ(f, true);
}

TEST(TestCaseName, Test4) {
    restore restore1(12, 40, 0.3, 9, 7);
    restore restore2(12, 40, 0.4, 9, 7);
    restore1.job();   
    restore2.job();
    bool f = false;
    if (restore1.middle > restore2.middle) 
        f = true;
    EXPECT_EQ(f, false);
}

TEST(TestCaseName, Test5) {
    restore restore(12, 40, 0.3, 9, 7);
    restore.job();
    restore.stat();
    bool f = false;
    if (restore.can < 1) 
        f = true;
    EXPECT_EQ(f, true);
}
