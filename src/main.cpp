#include <string>
#include <iostream>

#include "task.h"

int main() {
    setlocale(LC_ALL, "rus");

    Shop shop(4, 20, 700, 7, 5);
    std::string out = shop.Modelling();
    std::cout << out;
    return 0;
}
