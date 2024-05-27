#include "../include/task.h"
#include <gtest/gtest.h>

TEST(SupermarketTest, CustomerFlowRate) {
    Supermarket supermarket(5, 10, 1, 10, 20);
    supermarket.runSimulation(10); // Shorter simulation for test purposes

    ASSERT_GT(supermarket.getCustomersServed(), 0);
    ASSERT_GT(supermarket.getCustomersNotServed(), 0);
}

TEST(SupermarketTest, CashierWorkIdleTime) {
    Supermarket supermarket(5, 10, 1, 10, 20);
    supermarket.runSimulation(10); // Shorter simulation for test purposes

    for (const auto& cashier : supermarket.getCashiers()) {
        ASSERT_GT(cashier->getTotalWorkTime(), 0);
        ASSERT_GT(cashier->getTotalIdleTime(), 0);
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
