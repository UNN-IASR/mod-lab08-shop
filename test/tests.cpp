// Copyright 2024 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(TestRandom, TestRandomOfTimeAndProducts) { 
    //проверяем, соблюдаются ли значения интенсивности клиентов и количества продуктов
    //при использовании рандомной генерации на основе нормального распределения
    int checkout_count = 3;
    int product_processing_time = 50;
    int max_line_length = 5; 
    double intensity_customer = 20; 
    double avarage_count_of_products = 5; 
    int time_of_work = 15 * 1000;

    Model model(checkout_count, intensity_customer, product_processing_time,
        avarage_count_of_products, max_line_length);
    model.Run(time_of_work);

    double eps1 = 0.075;
    double eps2 = 0.05;

    EXPECT_TRUE(abs(1-model.get_actual_intensity_of_customers()/intensity_customer) < eps1);
    EXPECT_TRUE(abs(1-model.get_actual_avarage_count_of_products()/avarage_count_of_products) < eps2);
}

TEST(TestCalculate, TestCalculateOfTheoreticalFormulas) {
    //проверяем, правильно ли генерируется рандмное число
    int checkout_count = 3;
    int product_processing_time = 50;
    int max_line_length = 2;
    double intensity_customer = 20;
    double avarage_count_of_products = 6;
    

    Model model(checkout_count, intensity_customer, product_processing_time,
        avarage_count_of_products, max_line_length);
    
    double prob_of_rej_сalc_manually = (double) 144 / 277;

    int a = 1000000;
    EXPECT_EQ(round(model.get_theoretical_probability_of_rejection() * a), round(prob_of_rej_сalc_manually * a));
    EXPECT_EQ(round(model.get_theoretical_relative_throughput() * a), round((1 - prob_of_rej_сalc_manually) * a));
    EXPECT_EQ(round(model.get_theoretical_absolute_throughput() * a), round((1 - prob_of_rej_сalc_manually) * 20 * a));
}

TEST(TestShop, TestShopWork) {
    
    int checkout_count = 1;
    int product_processing_time = 100;
    int max_line_length = 1;
    Shop shop(checkout_count, product_processing_time, max_line_length);

    Customer c1, c2;
    c1.id = 1;
    c2.id = 2;
    c1.product_count = 4;
    c2.product_count = 6;

    shop.Open();
    shop.Handle_customer(c1);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    shop.Handle_customer(c2);
    shop.Close();

    double eps1 = 0.02;
    double eps2 = 35;

    EXPECT_TRUE(shop.get_work_time()-1000 <eps2);
    EXPECT_EQ(shop.get_served_customers_count(),2);
    EXPECT_EQ(shop.get_unserved_customers_count(),0);
    EXPECT_TRUE(abs(shop.get_avarage_line_length() - 0.2) < eps1);
    EXPECT_TRUE(shop.get_avarage_chechout_downtime() < eps1);
    EXPECT_TRUE(shop.get_avarage_checkout_work_time() - 1000 < eps2);
    EXPECT_TRUE(shop.get_avarage_customer_time() - 600 < eps2);

}

TEST(TestModel, TestActualResults) {
    
    int checkout_count = 1;
    int product_processing_time = 100;
    int max_line_length = 2;
    double intensity_customer = 10;
    double avarage_count_of_products = 5;
    int time_of_work = 500;

    Model model(checkout_count, intensity_customer, product_processing_time,
        avarage_count_of_products, max_line_length);

    for (int i = 0; i < 50; i++) {
        model.Run(time_of_work);

        EXPECT_TRUE((model.get_actual_probability_of_rejection() == 0) ||
            (model.get_actual_probability_of_rejection() == 0.2) ||
            (model.get_actual_probability_of_rejection() == 0.25) ||
            (model.get_actual_probability_of_rejection() == (double)1 / 3) ||
            (model.get_actual_probability_of_rejection() == 0.4) ||
            (model.get_actual_probability_of_rejection() == 0.5));
        EXPECT_TRUE((model.get_actual_relative_throughput() == 1) ||
            (model.get_actual_relative_throughput() == 0.8) ||
            (model.get_actual_relative_throughput() == 0.75) ||
            (model.get_actual_relative_throughput() == (double)2 / 3) ||
            (model.get_actual_relative_throughput() == 0.6) ||
            (model.get_actual_relative_throughput() == 0.5));
    }
}

TEST(TestModel, TestRandomNumber) {

    int checkout_count = 1;
    int product_processing_time = 100;
    int max_line_length = 2;
    double intensity_customer = 10;
    double avarage_count_of_products = 5;

    Model model(checkout_count, intensity_customer, product_processing_time,
        avarage_count_of_products, max_line_length);

    for (int i = 50; i < 100; i++) {
        int rand = model.get_random_number(i, (double)50/i);
        EXPECT_TRUE((rand > 0) && (rand < i*2));
    }
}
