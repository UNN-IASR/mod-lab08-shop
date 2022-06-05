// Copyright 2022 UNN-IASR
#include <iostream>
#include "task.h"

int main() {
    Shop shop(1, 10, 100, 1, 1);
    shop.Shopping();
    int s = shop.servedClients + shop.rejectedClients;
}
