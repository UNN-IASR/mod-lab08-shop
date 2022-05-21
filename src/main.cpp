// Copyright 2022 UNN-IASR
#include <iostream>
#include "task.h"

int main() {
    Shop shop(0, 60, 400, 3, 6);
    shop.Work();
    int s = shop.servedClients + shop.rejectedClients;
}
