// Copyright 2022 UNN-IASR
#include <iostream>
#include "task.h"

int main() {
    Shop shop(2, 15, 100, 5, 2);
    shop.Work();
    int s = shop.servedClients + shop.rejectedClients;
}
