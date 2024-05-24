#include "../include/task.h"
#include <iostream>

int main() {

    Shop shop(7,40,0.2,7,5);
    shop.start_work();
    shop.statistics();
    return 0;
}
