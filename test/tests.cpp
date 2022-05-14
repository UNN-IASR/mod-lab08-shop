// Copyright 2022 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(TEST1, test1) {
    int cashboxes = 2;
    int intensity = 30;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    int result = supermarket.getAmountOfNotServedClients();
    ASSERT_EQ(result, 0);
}

TEST(TEST2, test2) {
    int cashboxes = 2;
    int intensity = 30;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    int result = supermarket.getAmountOfServedClients();
    ASSERT_EQ(result, 4);
}

TEST(TEST3, test3) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getAverageQueueLength();
    ASSERT_EQ(result, 3);
}

TEST(TEST4, test4) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getAverageClientTime();
    ASSERT_TRUE(result > 30);
}

TEST(TEST5, test5) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
    
    ASSERT_TRUE(supermarket.getAverageCashboxWorktime() > supermarket.getAverageClientTime());
}

TEST(TEST6, test6) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getAverageCashboxDowntime();
    ASSERT_EQ(result, 0);
}

TEST(TEST7, test7) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getPrej();
    ASSERT_EQ(result, 0.2);
}

TEST(TEST8, test8) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getQ();
    ASSERT_EQ(result, 0.8);
}

TEST(TEST9, test9) {
    int cashboxes = 1;
    int intensity = 10;
    int speed = 10;
    int averageProducts = 3;
    int clients = 4;
  
    Shop supermarket(cashboxes, intensity, speed, averageProducts, clients);
    supermarket.startShopping();
  
    double result = supermarket.getA();
    ASSERT_EQ(result, 8);
}
