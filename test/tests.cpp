// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"
TEST(ClientTest, TestConstructor) {
    Client client(5, 2, 3);
    EXPECT_EQ(client.time_v_ocheredi, 3);
}
