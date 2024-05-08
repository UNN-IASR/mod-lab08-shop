#include "../include/task.h"

int Fact(int n) {
    if(n == 0)
        return 1;
    else
        return n * Fact(n - 1);
}

int main() {
    int cashDeskCount = 3; 
    double customerIntensity = 40;
    int productProcessingSpeed = 50;
    int avgProductCount = 3;
    int maxQueueLength = 5;
    int countCustomer = 1000;

    double mu = 1000.0 / productProcessingSpeed / avgProductCount;
    double ro = customerIntensity / mu;

    double P0 = 0;
    for(int i = 0; i <= cashDeskCount; i++) P0 += pow(ro, i) / Fact(i);
    for(int i = 1; i <= maxQueueLength; i++) P0 += pow(ro, cashDeskCount + i) / (pow(cashDeskCount, i) * Fact(cashDeskCount));
    P0 = 1/P0;
    double Pq = P0 * (pow(ro, cashDeskCount) / Fact(cashDeskCount)) * ((1 - pow(ro/cashDeskCount, maxQueueLength)) / (1 - ro/cashDeskCount));
    double Prej = P0 * pow(ro, cashDeskCount+maxQueueLength) / (pow(cashDeskCount, maxQueueLength) * Fact(cashDeskCount));
    double Q = 1 - Prej;
    double A = customerIntensity * Q;

    std::cout << "\nReject prob: " << Prej
            << "\nRel throughput: " << Q
            << "\nAbs throughput: " << A
            << "\nSystem load: " << ro
            << "\nSystem idle prob: " << P0
            << "\nQueue prob: " << Pq
            << std::endl << std::endl;

    Store store(cashDeskCount, customerIntensity, productProcessingSpeed, avgProductCount, maxQueueLength, countCustomer);
    store.simulate();

    std::cout << "Accepted customer: " << store.acceptedCustomer
            << "\nRejected customer: " << store.rejectedCustomer 
            << "\nReject prob: " << (double)store.rejectedCustomer / countCustomer
            << "\nRel throughput: " << (double)store.acceptedCustomer / countCustomer
            << "\nAbs throughput: " << (double)store.acceptedCustomer / countCustomer * customerIntensity
            << "\nAvg queue length: " << store.sumQueueLength / countCustomer
            << "\nAvg busy cashier time: " << store.busyCashierTime / cashDeskCount / 1000 << " sec"
            << "\nAvg passive cashiere time: " << (store.storeTime - store.busyCashierTime / cashDeskCount) / 1000 << " sec"
            << "\nAvg time in queue: " << (double)store.timeInQueue / store.acceptedCustomer / 1000 << " sec"
            << std::endl;

    return 0;
}
