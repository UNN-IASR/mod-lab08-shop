#include "../include/task.h"

int factorial(int n) {
    if (n <= 1) return 1;
    return n * factorial(n-1);
}

int main() {
    int chashboxesCount = 3;
    int queueSize = 10;
    int customerRate = 20;
    int cashiersSpeed = 50;
    int customerCart = 4;
    int clientNum = 100;

    int n = chashboxesCount;
    int m = queueSize;
    double lambda = customerRate;
    double mu = 1000.0 / cashiersSpeed / customerCart;
    double ro = lambda / mu;
    double P0 = 0;
    for (int k = 0; k <= n; k++) P0 += pow(ro, k) / factorial(k);
    for (int k = 1; k <= m; k++) P0 += pow(ro, n+k) / (pow(n, k) * factorial(n));
    P0 = pow(P0, -1);
    double Pq = (pow(ro, n)/factorial(n)) * ((1-pow(ro/n, m)) / (1-(ro/n))) * P0;
    double Prej = P0 * pow(ro, n+m) / (pow(n, m) * factorial(n));
    double Q = 1 - Prej;
    double A = lambda * Q;
    double Lq = (pow(ro, n+1)/(n*factorial(n))) * (1-pow(ro/n, m)*(1+m*(1-ro/n))) / pow(1-ro/n, 2) * P0;
    double t = (Lq / lambda) + (Q / mu);

    std::cout << "Reject Probability: " << Prej
            << "\nRelative Throughput: " << Q
            << "\nAbsolute Throughput: " << A
            << "\nAverage Customers Count In Queue: " << Lq
            << "\nAverage Customer Time In Store: " << t
            << std::endl << std::endl;

    Market market(chashboxesCount, cashiersSpeed, customerRate, customerCart, queueSize);
    market.Work(100);

    std::cout << "Reject Probability: " << market.rejectedCustomer / clientNum
            << "\nRelative Throughput: " << market.acceptedCustomer / clientNum
            << "\nAbsolute Throughput: " << market.acceptedCustomer * customerRate / clientNum
            << "\nAverage Customers Count In Queue: " << market.AvgQueueLength / clientNum
            << "\nAverage Customer Time In Store: " << market.AvgClientTime / 1000 / market.acceptedCustomer
            << std::endl;
}