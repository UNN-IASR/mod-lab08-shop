// Copyright 2022 UNN-IASR
#include "../include/task.h"
#include <iostream>

int main()
{
    int cass_number = 5;
    int max_num_of_clients = 120;
    int clients_intensity = 15;
    int serving_speed = 30;
    int average_item_num = 10;
    int max_line_len = 5;
    Market FiveyorOchka(cass_number, max_num_of_clients, clients_intensity, serving_speed, average_item_num, max_line_len);
    FiveyorOchka.start();
   
    std::cout << std::endl << std::endl << "STATISTICS" << std::endl << std::endl;
    std::cout << "Unserved clients: " << FiveyorOchka.getAmountOfUnservedClients() << std::endl;
    std::cout << "Served clients: " << FiveyorOchka.getAmountOfServedClients() << std::endl;
    std::cout << "Average line length: " << FiveyorOchka.getAverageLineLength() << std::endl;
    std::cout << "Average waiting time: " << FiveyorOchka.getAverageWaitingTimeForClient() << std::endl;
    std::cout << "Average worktime: " << FiveyorOchka.getAverageWorkTime() << std::endl;
    std::cout << "Average downtime: " << FiveyorOchka.getAverageDownTime() << std::endl;
}
