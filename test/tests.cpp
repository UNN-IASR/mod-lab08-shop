// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"
TEST(Test, Client) {
    Client client(5, 2, 3);
    EXPECT_EQ(client.time_v_ocheredi, 3);
}
TEST(KassaTest, TestObslugivanie) {
    Kassa kassa;
    Client client(5, 2, 3);
    std::thread t1(&Kassa::obslugivanie, &kassa, &client);
    t1.join();
    EXPECT_TRUE(kassa.polnaya);
    //EXPECT_EQ(kassa.getWaitTime(), client.getTimeObslugivania());
}
