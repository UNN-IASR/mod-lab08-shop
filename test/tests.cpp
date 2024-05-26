// Copyright 2021 GHA Test Team
#include "task.h"
#include <gtest/gtest.h>

TEST(SupermarketTest, TheoreticalCalculations) {
    int num_checkouts = 5;
    int customer_arrival_rate = 1; // покупателей в секунду
    int processing_speed = 1; // секунд на товар
    int avg_cart_size = 10; // среднее количество товаров
    int max_queue_length = 10;

    Supermarket supermarket(num_checkouts, customer_arrival_rate, processing_speed, avg_cart_size, max_queue_length);

    double P0 = supermarket.calculateTheoreticalP0();
    double Prej = supermarket.calculateTheoreticalPrej(P0);
    double Q = supermarket.calculateTheoreticalQ(Prej);
    double A = supermarket.calculateTheoreticalA(Q);

    ASSERT_NEAR(P0, 0.021, 0.001); // предполагаемое значение для P0
    ASSERT_NEAR(Prej, 0.002, 0.001); // предполагаемое значение для Prej
    ASSERT_NEAR(Q, 0.998, 0.001); // предполагаемое значение для Q
    ASSERT_NEAR(A, 0.998, 0.001); // предполагаемое значение для A
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
