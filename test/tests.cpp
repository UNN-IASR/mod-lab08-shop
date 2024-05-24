#include <gtest/gtest.h>
#include "../include/task.h"

TEST(Test, Test1) {
    Shop shop(5, 19, 0.2, 5, 4);
    shop.start_work();
    EXPECT_EQ(shop.clients_served + shop.clients_unserved, 19);
}

TEST(Test, Test2) {
    Shop shop(3, 5.0, 2.0, 10.0, 10);

    ASSERT_EQ(3, shop.count_cash_registers);
    ASSERT_EQ(5.0, shop.intensity_client_flow);
    ASSERT_EQ(2.0, shop.speed_processing_goods_checkout);
    ASSERT_EQ(10.0, shop.average_count_products_cart);
    ASSERT_EQ(10, shop.max_len_queue);
}

TEST(Test, Test3) {
    Shop shop(3, 5.0, 2.0, 10.0, 10);
    Client client;
    client.count_products = 5;

    shop.cash_obrabotka(0, client, 2.0);

    ASSERT_EQ(2, shop.cash_register[0].time_waiting_cr);
    ASSERT_EQ(3, client.time_service);
}
