// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(TestRegister, TestRegisterTimeWork) {
    Client client(1);
    Register registers;

    int speed_service = 1000;
    registers.RunService(client, speed_service);

    double delta = registers.GetWorkTime().count() - 1000;

    std::cout << std::abs(delta);
    EXPECT_TRUE(std::abs(delta) < 500);
}

TEST(TestRegister, TestRegisterClientCount) {
    Client client(1);
    Client client2(1);

    Register registers;

    int speed_service = 1000;
    registers.RunService(client, speed_service);
    registers.RunService(client2, speed_service);

    EXPECT_EQ(registers.GetCountClient(), 2);
}

TEST(TestStatistics, TestStatistics1) {
    int count_registers = 2;
    double intensity = 2;
    int speed_service = 900;
    int count_product = 1;
    int maxLen_queue = 2;

    std::chrono::milliseconds time_run(2000);

    Shop shop(count_registers, intensity, speed_service, count_product, maxLen_queue);
    shop.Run(time_run);

    Statistics statistics = shop.GetStatistics();

    EXPECT_EQ(statistics.count_registers, 2);

    EXPECT_EQ(statistics.count_failures, 0);
    EXPECT_EQ(statistics.count_visitors, statistics.count_serviced);
}

TEST(TestStatistics, TestStatistics2) {
    int count_registers = 2;
    double intensity = 2;
    int speed_service = 900;
    int count_product = 1;
    int maxLen_queue = 2;

    std::chrono::milliseconds time_run(2000);

    Shop shop(count_registers, intensity, speed_service, count_product, maxLen_queue);
    shop.Run(time_run);

    Statistics statistics = shop.GetStatistics();

    double delta = 0.1;

    EXPECT_TRUE(std::abs(statistics.Prej_real() - 0) < delta);
    EXPECT_TRUE(std::abs(statistics.Q_real() - 1) < delta);
    EXPECT_TRUE(std::abs(statistics.A_real() - 1.7) < delta);

    EXPECT_TRUE(std::abs(statistics.Prej_theor() - 0.22) < delta);
    EXPECT_TRUE(std::abs(statistics.Q_theor() - 0.77) < delta);
    EXPECT_TRUE(std::abs(statistics.A_theor() - 1.55) < delta);
}
