#include "task.h"

int main() {
    Shop sh(5, 5, 0.15, 5, 5, 25);
    sh.run();
    std::cout << sh.statistic.served <<std::endl;
    std::cout << sh.statistic.rejected << std::endl;
    return 0;
}