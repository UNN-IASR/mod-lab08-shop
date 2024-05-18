// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"
TEST(Client, Test1) {
    Client client(5, 2, 3);
    EXPECT_EQ(client.time_v_ocheredi, 3);//остальные значения рандомно определяются
}
TEST(KassaTest, Test2) {
    Kassa kassa;
    Client client(5, 2, 3);
    std::thread t1(&Kassa::obslugivanie, &kassa, &client);
    t1.join();
    EXPECT_FALSE(kassa.polnaya);//так как сразу после обслуживания она становится false
}
TEST(SupermarketTest, Test3) {
    Supermarket supermarket1(3, 10, 2, 5, 10);
    Client client(5, 2, 3);
    supermarket1.addclient(&client);
    EXPECT_EQ(supermarket1.clients.size(), 1);
}
TEST(SupermarketTest, Test4) {
    Supermarket supermarket1(3, 10, 2, 5, 10);
    Client client(5, 2, 3);
    supermarket1.addclient(&client);
    EXPECT_EQ(supermarket1.clients.front()->time_v_ocheredi, 3);
}
TEST(SupermarketTest, Test5) {
    Supermarket supermarket1(3, 10, 2, 5, 10);
    EXPECT_EQ(supermarket1.count_kass, 3);
    EXPECT_EQ(supermarket1.intensivnost_potoka_pokupatelei, 10);
    EXPECT_EQ(supermarket1.skorost_obrabotki_tovara, 2);
    EXPECT_EQ(supermarket1.srednee_kol_productov, 5);
    EXPECT_EQ(supermarket1.max_dlina_ocheredi, 10);
}
