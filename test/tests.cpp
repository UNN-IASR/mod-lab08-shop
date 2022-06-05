// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"



TEST(TEST1, 1) {
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len); 
    FiveyorOchka.start();
    ASSERT_GT(FiveyorOchka.getAmountOfUnservedClients(), 10);
}

TEST(TEST2, 2) {
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len);
    FiveyorOchka.start();
    ASSERT_GT(FiveyorOchka.getAmountOfServedClients(), 15);
}

TEST(TEST3, 3) {
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len);
    FiveyorOchka.start();
    ASSERT_GT(FiveyorOchka.getAverageLineLength(), 1);
}

TEST(TEST4, 4) {
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len);
    FiveyorOchka.start();
    ASSERT_GT(FiveyorOchka.getAverageWaitingTimeForClient(), 2);
}

TEST(TEST5, 5) {
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len);
    FiveyorOchka.start();
    ASSERT_GT(FiveyorOchka.getAverageWorkTime(), 17);
}
