#include "task.h"

int factorial(int n){
    if (n == 0)
        return 1;
    return n*factorial(n-1);
}

int main() {
    int cash = 3; //количество касс (каналов обслуживания)
    double shoppersIntensity = 6; //интенсивность потока покупателей (заявки на обслуживание)
    double productCashTime = 0.5; //скорость обработки товара на кассе
    double avgProducts = 1; //среднее количество товаров в тележке покупателя
    int maxQueueLength = 5; //максимальная длина очереди
    int customers = 1000; //покупателей за смену
    Shop sh(cash, shoppersIntensity, productCashTime, avgProducts, maxQueueLength, customers);
    sh.run();
    std::cout << "served " << sh.statistic.served <<std::endl;
    std::cout << "rejected " << sh.statistic.rejected << std::endl;
    std::cout << "avgServeTime " << sh.statistic.avgServeTime <<std::endl;
    std::cout << "avgQueueLength " << sh.statistic.avgQueueLength << std::endl;
    std::cout << "avgWorkTime " << sh.statistic.avgCashWorkTime <<std::endl;
    std::cout << "avgWaitTime " << sh.statistic.avgCashWaitTime << std::endl;

    double ro = shoppersIntensity * productCashTime * avgProducts;
    double P0 = 1;
    for (int i = 1; i <= cash; i++)
        P0 += pow(ro, i) / factorial(i);
    for(int i = 1; i <= maxQueueLength; i++)
        P0 += pow(ro, cash + i) / (pow(cash, i) * factorial(cash));
    P0 = pow(P0, -1);
    double Pn = pow(ro, cash + maxQueueLength) * P0 / (pow(cash, maxQueueLength) * factorial(cash));

    std::cout << "Theoretical:" << std::endl;
    std::cout << "rejection P " << round(Pn * 10000) / 10000.0 << std::endl;
    std::cout << "relative " << round((1 - Pn) * 10000) / 10000.0 << std::endl;
    std::cout << "absolete " << round(shoppersIntensity * (1 - Pn) * 10000) / 10000.0 << std::endl;

    std::cout << "Practical:" << std::endl;
    std::cout << "rejection P " << round((double)sh.statistic.rejected / customers * 10000) / 10000.0 << std::endl;
    std::cout << "relative " << round((1 - (double)sh.statistic.rejected / customers) * 10000) / 10000.0 << std::endl;
    std::cout << "absolete " << round(shoppersIntensity * (1 - (double)sh.statistic.rejected / customers) * 10000) / 10000.0 << std::endl;

    return 0;
}