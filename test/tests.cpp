// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"


TEST(TEST1, create_customer) {
    int cashboxes_number = 5;
    int max_num_of_customers = 100;
    int customers_intensity = 20;
    int serving_speed = 15;
    int average_product_num = 10;
    int max_line_len = 5;
    Supermarket supermarket(cashboxes_number, max_num_of_customers, customers_intensity, serving_speed, average_product_num, max_line_len);
    Customer* result = supermarket.getCustomer();
    ASSERT_EQ(result->check.size(), 10);
}

TEST(TEST2, get_served_and_unserved_customers_amount) {
    int cashboxes_number = 5;
    int max_num_of_customers = 10;
    int customers_intensity = 2;
    int serving_speed = 5;
    int average_product_num = 10;
    int max_line_len = 10;
    Supermarket supermarket(cashboxes_number, max_num_of_customers, customers_intensity, serving_speed, average_product_num, max_line_len);
    supermarket.start();
    ASSERT_EQ(supermarket.getAmountOfUnservedCustomers(), 1);
}

TEST(TEST3, get_average_line_length) {
    int cashboxes_number = 5;
    int max_num_of_customers = 10;
    int customers_intensity = 2;
    int serving_speed = 5;
    int average_product_num = 10;
    int max_line_len = 10;
    Supermarket supermarket(cashboxes_number, max_num_of_customers, customers_intensity, serving_speed, average_product_num, max_line_len);
    supermarket.start();
    ASSERT_EQ(supermarket.getAverageWorkTime(), 100);
}

TEST(TEST4, get_average_downtime) {
    int cashboxes_number = 1;
    int max_num_of_customers = 5;
    int customers_intensity = 1;
    int serving_speed = 20;
    int average_product_num = 5;
    int max_line_len = 1;
    Supermarket supermarket(cashboxes_number, max_num_of_customers, customers_intensity, serving_speed, average_product_num, max_line_len);
    supermarket.start();
    ASSERT_EQ(supermarket.getAverageDownTime(), 0);
}

TEST(TEST5, get_average_waiting_time_for_customer) {
    int cashboxes_number = 5;
    int max_num_of_customers = 10;
    int customers_intensity = 2;
    int serving_speed = 5;
    int average_product_num = 10;
    int max_line_len = 10;
    Supermarket supermarket(cashboxes_number, max_num_of_customers, customers_intensity, serving_speed, average_product_num, max_line_len);
    supermarket.start();
    EXPECT_EQ(supermarket.getAverageWorkTime(), 100);
}