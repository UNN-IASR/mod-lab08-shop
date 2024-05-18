// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"
TEST(Test, Client) {
    Client client(5, 2, 3);
    EXPECT_EQ(client.time_v_ocheredi, 3);
}
