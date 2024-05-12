// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include <vector>
#include "../include/task.h"

TEST(task, basic) {
    Task task(4, 0);
    ASSERT_EQ(task.begin_begin, 0);
    ASSERT_EQ(task.time, 4);
    ASSERT_FALSE(task.is_assigned());
}

TEST(task, assign) {
    Task task(2, 0);
    task.assign(3, 1);
    ASSERT_TRUE(task.is_assigned());
    ASSERT_TRUE(task.is_cashbox_available(6));
    ASSERT_TRUE(task.is_cashbox_available(5));
    ASSERT_FALSE(task.is_cashbox_available(4));
}

TEST(task, data) {
    Task task(2, 0);
    task.assign(3, 1);
    ASSERT_EQ(task.cashbox, 1);
    ASSERT_EQ(task.begin, 3);
}

TEST(task, find_free) {
    std::vector<Task> tasks;
    Task task1(5, 0);
    task1.assign(0, 0);
    tasks.push_back(task1);
    Task task2(2, 0);
    task2.assign(0, 0);
    tasks.push_back(task2);
    ASSERT_EQ(1, Task::find_free(2, 3, tasks));
}
