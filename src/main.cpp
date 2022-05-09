  // Copyright 2022 UNN-IASR
#include <ctime>
#include "market.h"

int main() {
srand(time(nullptr));
Supermarket* supermarket = new Supermarket(20, 40, 500, 5, 20, 1);
supermarket->run();
std::cout << "Served buyers: "
<< supermarket->ServedBuyers() << "\n";
std::cout << "Not served buyers: "
<< supermarket->NotServedBuyers() << "\n";
std::cout << "Average buyers on queue: "
<< supermarket->AverageBuyersOnQueue() << "\n";
std::cout << "Average time in queue: "
<< supermarket->InQueue() << " sec\n";
std::cout << "Average time on cash register: "
<< supermarket->OnCashBox() << " sec\n";
std::cout << "Time during which the cash register is idle "
<< supermarket->TimeCashBoxStop() << " sec\n";
std::cout << "Time during which the cash register is work "
<< supermarket->TimeCashBoxWork() << " sec\n";
std::cout << "Failure probability "
<< supermarket->FailureProbability() << "\n";
std::cout << "Relative throughput "
<< supermarket->RelativeThroughput() << "\n";
std::cout << "Absolute bandwidth "
<< supermarket->AbsoluteBandwidth() << "\n";
return 0;
}
