#include <iostream>
#include "task.h"

int main()
{
    int count_registers = 2;
    double intensity = 2;
    int speed_service = 900;
    int count_product = 1;
    int maxLen_queue = 2;

    std::chrono::milliseconds time_run(2000);

    Shop shop(count_registers, intensity, speed_service, count_product, maxLen_queue);
    shop.Run(time_run);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << shop.GetStatistics().ToString() << std::endl;
}