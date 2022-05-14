#include <iostream>
#include "task.h"

int main()
{
    //countcashbox requestrate servicespeed avgitems maxqueue
    Shop shop(2, 30, 200, 5, 4); 
    shop.Simulation();
    int s = shop.completedClients + shop.rejectedClients;
}
