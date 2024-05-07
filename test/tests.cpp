//Copyright Maz 2024
#include "../include/task.h"
#include "gtest/gtest.h"

TEST(Test1, ClientMake) {
  Client cl = Client(3, 1);
  int count = cl.get_count();
  EXPECT_EQ(count, 3);
}

TEST(Test2, CashMake) {
  Cashier cash = Cashier(50);
  bool check = cash.is_serv();
  EXPECT_EQ(check, false);
}

TEST(Test3, ClientAdd) {
  Cashier cash = Cashier(50);
  Client cl = Client(4, 2);
  cash.client_add(cl);
  int count = cash.queue_size();
  EXPECT_EQ(count, 1);
}

TEST(Test4, CashChoosing) {
  Cashier cash = Cashier(50);
  Client cl = Client(4, 2);
  int num = CashChoose(&cash, 1, 5);
  EXPECT_EQ(num, 0);
}

TEST(Test5, TimePushingExample) {
  Cashier cash = Cashier(50);
  for (int i = 0; i < 10; i++) {
    cash.work_push((double)i);
  }
  std::vector<double> times = cash.Work_Times();
  EXPECT_EQ(times[2], (double)2);
}
