#include "task.h"
#include <iostream>

int main()
{
    Store store(5, 50, 0.1, 10, 10);
    store.Run();
    store.Statistics();

    return 0;
}