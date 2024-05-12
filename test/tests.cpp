// Copyright 2024 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(StoreTest, GenerateRandomNumberTest) {
    int min = 1;
    int max = 10;
    int random_number = generateRandomNumber(min, max);
    EXPECT_TRUE(random_number >= min && random_number <= max);
}

TEST(StoreTest, GetCurrentTimeTest) {
    Store store(1, 1, 1, 1);
    int current_time = store.getCurrentTime();
    EXPECT_TRUE(current_time > 0);
}

TEST(StoreTest, ProcessTest1) {
    Store store(1, 1, 1, 1);
    store.simulate(2, 2);
    store.printStatistics();
    Statics stat = store.printStatistics();
    EXPECT_EQ(stat.served_customers, 1);
}

TEST(StoreTest, ProcessTest2) {
    Store store(0, 10, 1, 10);
    store.simulate(10, 2);
    Statics stat = store.printStatistics();
    EXPECT_EQ(stat.served_customers, 0);
    EXPECT_EQ(stat.failureProbability, 1);
    EXPECT_EQ(stat.relativeThroughput, 0);
}

TEST(StoreTest, ProcessTest3) {
    Store store(1, 1, 1, 0);
    store.simulate(2, 2);
    store.printStatistics();
    Statics stat = store.printStatistics();
    EXPECT_EQ(stat.served_customers, 0);
}

TEST(StoreTest, FailureProbability) {
    Store store(4, 10, 1, 10);
    store.simulate(10, 2);
    Statics stat = store.printStatistics();
    EXPECT_TRUE(stat.failureProbability <= 1 && stat.failureProbability >= 0);
}

TEST(StoreTest, RelativeThroughput) {
    Store store(4, 10, 1, 10);
    store.simulate(10, 2);
    Statics stat = store.printStatistics();
    EXPECT_TRUE(stat.relativeThroughput <= 1 && stat.relativeThroughput >= 0);
}

TEST(StoreTest, RealVsTeoretic1) {
    int number_cash_registers = 2;
    int customer_intensity = 10;
    double speed = 10;
    int max_queue_length = 10;
    int simulation_time = 20;
    double average_items = 3;

    Store store(number_cash_registers, customer_intensity, speed, max_queue_length);
    store.simulate(20, average_items);
    Statics stat1 = store.printStatistics();
    Statics stat2 = store.printTeoreticStatics(customer_intensity, speed, number_cash_registers, max_queue_length, average_items);
    double eps = 0.2;
    EXPECT_TRUE(abs(stat1.relativeThroughput - stat2.relativeThroughput) <= eps);
}

TEST(StoreTest, RealVsTeoretic2) {
    int number_cash_registers = 2;
    int customer_intensity = 10;
    double speed = 10;
    int max_queue_length = 10;
    int simulation_time = 20;
    double average_items = 3;

    Store store(number_cash_registers, customer_intensity, speed, max_queue_length);
    store.simulate(20, average_items);
    Statics stat1 = store.printStatistics();
    Statics stat2 = store.printTeoreticStatics(customer_intensity, speed, number_cash_registers, max_queue_length, average_items);
    double eps = 0.2;
    EXPECT_TRUE(abs(stat1.failureProbability - stat2.failureProbability) <= eps);
}
