#include "task.h"
#include <random>
#include <ctime>

int main()
{
    unsigned int shop_size = 3;
    unsigned int queue_size = 10;
    unsigned int item_service_time = 100;
    unsigned int customers_count = 50;
    unsigned int customer_max_time = 100;
    unsigned int customer_min_time = 10;
    unsigned int max_items_count = 20;
    unsigned int min_items_count = 5;

    std::srand(std::time(nullptr));
    shop shop1{ queue_size,shop_size,std::chrono::milliseconds(item_service_time) };
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    for (int i = 0; i < customers_count; i++)
    {
        shop1.service(customer(i, min_items_count + rand() % (max_items_count - min_items_count)));
        std::this_thread::sleep_for(std::chrono::milliseconds(customer_min_time + std::rand() % (customer_max_time - customer_min_time)));
    }
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

    std::chrono::milliseconds total_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    shop1.close();
    shop1.print_stat((double)customers_count / (double)total_time.count());
}
