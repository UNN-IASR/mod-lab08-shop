#include "task.h"
#include <iostream>

int main()
{
    restore restore(8, 40, 0.25, 7, 5);
    restore.job();
    restore.stat();
    return 0;
}
