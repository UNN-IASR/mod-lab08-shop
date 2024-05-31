#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TestCaseName, Test1) {
    Restore restore(12, 34, 0.3, 9, 7);
    restore.job();
    EXPECT_EQ(restore.servis + restore.not_servis, 30);
}

TEST(TestCaseName, Test2) {
    Restore restore1(12, 34, 0.3, 9, 7);
    Restore restore2(12, 34, 0.3, 9, 7);
    restore1.job();
    restore2.job();
    bool f = false;
    if (restore1.servis < restore2.not_servis) 
        f = true;
    EXPECT_EQ(f,true);
}
TEST(TestCaseName, Test3) {
    Restore restore1(12, 34, 0.3, 9, 7);
    Restore restore2(12, 34, 0.4, 9, 7);
    restore1.job();
    restore2.job();
    bool f = true;
    if (restore1.servis > restore2.not_servis) 
        f = false;
    EXPECT_EQ(f, true);
}

TEST(TestCaseName, Test4) {
    Restore restore1(12, 34, 0.3, 9, 7);
    Restore restore2(12, 34, 0.4, 9, 7);
    restore1.job();   
    restore2.job();
    bool f = false;
    if (restore1.middle > restore2.middle) 
        f = true;
    EXPECT_EQ(f, false);
}

TEST(TestCaseName, Test5) {
    Restore restore(12, 34, 0.3, 9, 7);
    restore.job();
    restore.statisi();
    bool f = false;
    if (restore.can < 1) 
        f = true;
    EXPECT_EQ(f, true);
}
