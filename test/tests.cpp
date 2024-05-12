// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(TEST1, servedCustomers) {

  int cashRegisterAmount = 2;
  int flowRate = 500;
  int processingSpeed = 1000;
  int averageNumberOfProducts = 8;
  int maxQueueLength = 6;
  int maxBuyers = 10;

  Supermarket* supermarket = new Supermarket(cashRegisterAmount, flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
  supermarket->run();

  ASSERT_TRUE(supermarket->getServedCustomers() > cashRegisterAmount);
}

TEST(TEST2, notServedCustomers) {

  int cashRegisterAmount = 2;
  int flowRate = 500;
  int processingSpeed = 1000;
  int averageNumberOfProducts = 8;
  int maxQueueLength = 6;
  int maxBuyers = 5;

  Supermarket* supermarket = new Supermarket(cashRegisterAmount, flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
  supermarket->run();

  ASSERT_EQ(supermarket->getNotServedCustomers(),0);
}

TEST(TEST3, notServedCustomers) {

  int cashRegisterAmount = 2;
  int flowRate = 500;
  int processingSpeed = 1000;
  int averageNumberOfProducts = 8;
  int maxQueueLength = 6;
  int maxBuyers = 10;

  Supermarket* supermarket = new Supermarket(cashRegisterAmount, flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
  supermarket->run();

  ASSERT_EQ(supermarket->getNotServedCustomers(), 2);
}

TEST(TEST4, refuseProbability) {

  int cashRegisterAmount = 2;
  int flowRate = 500;
  int processingSpeed = 1000;
  int averageNumberOfProducts = 8;
  int maxQueueLength = 6;
  int maxBuyers = 10;

  Supermarket* supermarket = new Supermarket(cashRegisterAmount, flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
  supermarket->run();

  ASSERT_TRUE(supermarket->refuseProbability() < 0.5);
}

TEST(TEST5, serviceTime) {

  int cashRegisterAmount = 2;
  int flowRate = 500;
  int processingSpeed = 1000;
  int averageNumberOfProducts = 8;
  int maxQueueLength = 6;
  int maxBuyers = 3;

  Supermarket* supermarket = new Supermarket(cashRegisterAmount, flowRate, processingSpeed, averageNumberOfProducts, maxQueueLength, maxBuyers);
  supermarket->run();

  double time = (averageNumberOfProducts + 10) * processingSpeed;

  ASSERT_TRUE(supermarket->getServiceTime() < time);
}
