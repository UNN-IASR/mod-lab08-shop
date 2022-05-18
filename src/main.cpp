  // Copyright 2022 UNN-IASR
#include <ctime>
#include "market.h"

int main() {
srand(time(nullptr));
Market* supermarket = new Market(20, 40, 500, 5, 20, 1);
supermarket->run();
std::cout << "Served: "
<< supermarket->ServedB() << "\n";
std::cout << "Not served: "
<< supermarket->NServedB() << "\n";
std::cout << "Average buyers: "
<< supermarket->AverBuyersOnQ() << "\n";
std::cout << "Average time: "
<< supermarket->InQueue() << " ms\n";
std::cout << "Average time on cash register: "
<< supermarket->OnCash() << " ms\n";
std::cout << "Cash register is idle "
<< supermarket->TimeCashS() << " ms\n";
std::cout << "Cash register is work "
<< supermarket->TimeCashW() << " ms\n";
std::cout << "=====Theoretic results=====" << "\n";
std::cout << "Failure probability "
<< supermarket->FailProb() << "\n";
std::cout << "Relative throughput "
<< supermarket->RelThrough() << "\n";
std::cout << "Absolute bandwidth "
<< supermarket->AbsoluteBand() << "\n";
return 0;
}
