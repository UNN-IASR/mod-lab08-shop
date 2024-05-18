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
TEST(SupermarketTest, Test6) {
    Supermarket supermarket(3, 10, 2, 5, 10);
    std::vector<Client*> clients;
    for (int i = 0; i < 10; i++) {
        Client* client = new Client(5, 2, 3);
        clients.push_back(client);
    }
    for (auto client : clients) {
        supermarket.addclient(client);
    }
    supermarket.obrabativaem_ochered();
    EXPECT_EQ(supermarket.clients.size(), 0);
}
TEST(SupermarketTest, Test7) {
    Supermarket supermarket(3, 10, 2, 5, 10);
    std::vector<Client*> clients;
    for (int i = 0; i < 10; i++) {
        Client* client = new Client(5, 2, 3);
        clients.push_back(client);
    }
    for (auto client : clients) {
        supermarket.addclient(client);
    }
    supermarket.obrabativaem_ochered();
    EXPECT_EQ(supermarket.kol_obslug_clientov + supermarket.kol_neobslug_clientov, 10);
}
TEST(SupermarketTest, Test8) {
    Supermarket supermarket(3, 10, 2, 5, 10);
    std::vector<Client*> clients;
    for (int i = 0; i < 10; i++) {
        Client* client = new Client(5, 2, 3);
        clients.push_back(client);
    }
    for (auto client : clients) {
        supermarket.addclient(client);
    }
    supermarket.obrabativaem_ochered();
    EXPECT_EQ(supermarket.kol_obslug_clientov, 10);
}
TEST(SupermarketTest, Test9) {
    Supermarket supermarket(3, 10, 2, 5, 10);
    std::vector<Client*> clients;
    for (int i = 0; i < 10; i++) {
        Client* client = new Client(5, 2, 3);
        clients.push_back(client);
    }
    for (auto client : clients) {
        supermarket.addclient(client);
    }
    supermarket.obrabativaem_ochered();
    EXPECT_EQ(supermarket.kol_neobslug_clientov, 0);
}

