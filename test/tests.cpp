// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "task.h"

TEST(test_Stats_creation, Stats_init)
{
    Stats s;
    int served = s.served;
    int rejected = s.rejected;
    double avgQueueLength = s.avgQueueLength;
    double avgServeTime = s.avgServeTime;
    double avgCashWorkTime = s.avgCashWorkTime;
    double avgCashWaitTime = s.avgCashWaitTime;
    ASSERT_EQ(0, served);
    ASSERT_EQ(0, rejected);
    ASSERT_EQ(0, avgQueueLength);
    ASSERT_EQ(0, avgServeTime);
    ASSERT_EQ(0, avgCashWorkTime);
    ASSERT_EQ(0, avgCashWaitTime);
}

TEST (test_Stats_calculate, Stats_calc){
    Stats s;
    s.lifeTimes = {3, 3, 3};
    s.sumLengths = 2;
    s.countLenghts = 2;
    s.workTime = {4, 6, 5};
    s.waitTime = {1, 2, 3};

    s.calcResults();

    ASSERT_EQ(3, s.avgServeTime);
    ASSERT_EQ(1, s.avgQueueLength);
    ASSERT_EQ(5, s.avgCashWorkTime);
    ASSERT_EQ(2, s.avgCashWaitTime);
}

TEST (test_Shopper_lifetime, Shopper_life_clock) {
    Shopper s(1);

    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    int time = s.lifeTime();

    ASSERT_EQ(2, time);

}

//По какой-то причине git уходит на 6ти часовую попытку запустить магазин, хотя локально на эти тесты уходит едва ли больше 5ти секунд...

TEST (tets_Shop_all_served, Shop_100) {
    int cash = 10; //количество касс (каналов обслуживания)
    double shoppersIntensity = 6; //интенсивность потока покупателей (заявки на обслуживание)
    double productCashTime = 0.5; //скорость обработки товара на кассе
    double avgProducts = 1; //среднее количество товаров в тележке покупателя
    int maxQueueLength = 5; //максимальная длина очереди
    int customers = 5; //покупателей за смену
    Shop sh(cash, shoppersIntensity, productCashTime, avgProducts, maxQueueLength, customers);
    
    sh.run();

    ASSERT_EQ(customers, sh.statistic.served);
}

TEST (tets_Shop_none_served, Shop_0) {
    int cash = 0; //количество касс (каналов обслуживания)
    double shoppersIntensity = 6; //интенсивность потока покупателей (заявки на обслуживание)
    double productCashTime = 0.5; //скорость обработки товара на кассе
    double avgProducts = 1; //среднее количество товаров в тележке покупателя
    int maxQueueLength = 0; //максимальная длина очереди
    int customers = 5; //покупателей за смену
    Shop sh(cash, shoppersIntensity, productCashTime, avgProducts, maxQueueLength, customers);
    
    sh.run();

    ASSERT_EQ(customers, sh.statistic.rejected);
}

TEST (test_Shop_some_long_shift, Shop_long_shift){
    int cash = 10; //количество касс (каналов обслуживания)
    double shoppersIntensity = 6; //интенсивность потока покупателей (заявки на обслуживание)
    double productCashTime = 0.15; //скорость обработки товара на кассе
    double avgProducts = 15; //среднее количество товаров в тележке покупателя
    int maxQueueLength = 20; //максимальная длина очереди
    int customers = 100; //покупателей за смену
    Shop sh(cash, shoppersIntensity, productCashTime, avgProducts, maxQueueLength, customers);
    
    sh.run();

    ASSERT_TRUE(sh.statistic.served > 0);
}
