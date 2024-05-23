#include <iostream>
#include "task.cpp"

int main()  {
    Shop_working shop_working(2, 10, 5, 15, 4);
    shop_working.Start_sim(10);
    std::cout << "MAIN FINISH!!!😐";

    return 0; 
}