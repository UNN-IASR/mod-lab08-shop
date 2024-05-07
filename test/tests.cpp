// Copyright 2024 Danil Volkov

#include <gtest/gtest.h>
#include <string>
#include <iostream>
#include "../include/task.h"

class ShopTest : public testing::Test {};

TEST(ShopTest, SingleCashier) {
    setlocale(LC_ALL, "");
    Shop shop(1, 1, 1.0);
    Client client(&shop, 5);

    std::vector<std::thread> serverThreads;
    serverThreads.push_back(std::thread(&Shop::answer, &shop));

    client.send(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    client.send(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    client.send(3);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    shop.setAllCustomersProcessed();
    for (auto& t : serverThreads) {
        t.join();
    }

    EXPECT_EQ(shop.getRequestCount(), 3);
    EXPECT_EQ(shop.getProcessedCount(), 2);
    EXPECT_EQ(shop.getRejectedCount(), 1);
}

TEST(ShopTest, SingleCashier2) {
    setlocale(LC_ALL, "");
    Shop shop(1, 1, 5.0);
    Client client(&shop, 5);

    std::vector<std::thread> serverThreads;
    serverThreads.push_back(std::thread(&Shop::answer, &shop));

    client.send(1);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client.send(2);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    client.send(3);
    std::this_thread::sleep_for(std::chrono::milliseconds(4000));

    shop.setAllCustomersProcessed();
    for (auto& t : serverThreads) {
        t.join();
    }

    EXPECT_EQ(shop.getRequestCount(), 3);
    EXPECT_EQ(shop.getProcessedCount(), 3);
    EXPECT_EQ(shop.getRejectedCount(), 0);
}

TEST(ShopTest, MultipleCashiers) {
    setlocale(LC_ALL, "");
    Shop shop(2, 5, 5.0);
    Client client(&shop, 5);

    std::vector<std::thread> serverThreads;
    for (int i = 0; i < 2; ++i) {
        serverThreads.push_back(std::thread(&Shop::answer, &shop));
    }

    for (int id = 1; id <= 10; id++) {
        client.send(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    shop.setAllCustomersProcessed();
    for (auto& t : serverThreads) {
        t.join();
    }

    EXPECT_EQ(shop.getRequestCount(), 10);
    EXPECT_EQ(shop.getProcessedCount(), 8);
    EXPECT_EQ(shop.getRejectedCount(), 2);
}

TEST(ShopTest, MultipleCashiers2) {
    setlocale(LC_ALL, "");
    Shop shop(2, 5, 10.0);
    Client client(&shop, 5);

    std::vector<std::thread> serverThreads;
    for (int i = 0; i < 2; ++i) {
        serverThreads.push_back(std::thread(&Shop::answer, &shop));
    }

    for (int id = 1; id <= 5; id++) {
        client.send(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    shop.setAllCustomersProcessed();
    for (auto& t : serverThreads) {
        t.join();
    }

    EXPECT_EQ(shop.getRequestCount(), 5);
    EXPECT_EQ(shop.getProcessedCount(), 5);
    EXPECT_EQ(shop.getRejectedCount(), 0);
}

TEST(ShopTest, Statistic) {
    setlocale(LC_ALL, "");
    Shop shop(3, 5, 10.0);
    Client client(&shop, 10);

    std::vector<std::thread> serverThreads;
    for (int i = 0; i < 3; ++i) {
        serverThreads.push_back(std::thread(&Shop::answer, &shop));
    }

    for (int id = 1; id <= 20; id++) {
        client.send(id);
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    shop.setAllCustomersProcessed();
    for (auto& t : serverThreads) {
        t.join();
    }

    EXPECT_NEAR(shop.getAverageQueueLength(), 7.4, 1.1);
    EXPECT_NEAR(shop.getAverageQueueTime(), 0.9, 0.2);
    EXPECT_NEAR(shop.getAverageCashierTime(), 1.1, 0.2);
    EXPECT_NEAR(shop.getAverageCashierTime() - shop.getAverageQueueTime(), 0.15, 0.2);
    EXPECT_NEAR((double)shop.getRejectedCount() / shop.getRequestCount() * 100, 20, 1);
    EXPECT_NEAR((double)shop.getProcessedCount() / (shop.getProcessedCount() + shop.getRejectedCount()) * 100, 80, 1);
}

