#include "../include/task.h"
#include <iostream>

int main()
{
    Restore restore(8, 40, 0.25, 7, 5);
    restore.job();
    restore.stats();
    return 0;
}
