#include <iostream>
#include <cmath>

#include "task.h"

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    const int clientsCount = 100;
    const double clientIntensity = 10.0;
    const int queueLength = 3;
    const int cashboxesCount = 7;
    const int timeToProcessProduct = 100;
    const double avgProductsInCart = 5.0;

    // Calculating system parameters
    const double lambda = clientIntensity;
    const double mu = 1000.0 / timeToProcessProduct / avgProductsInCart;
    const double rho = mu / lambda;

    double P0 = 1;
    for (int i = 1; i <= cashboxesCount; i++) {
        P0 += std::pow(rho, i) / factorial(i);
    }
    for (int i = 1; i <= queueLength; i++) {
        P0 += std::pow(rho, cashboxesCount + i)
            / (std::pow(cashboxesCount, i) * factorial(i));
    }

    const double P_rej = std::pow(rho, cashboxesCount + queueLength) * P0
                         / (std::pow(cashboxesCount, queueLength) * factorial(cashboxesCount));
    const double Q = 1 - P_rej;
    const double A = lambda * Q;

    // Output system characteristics
    std::cout << "Вероятность отказа: " << P_rej << std::endl;
    std::cout << "Относительная пропускная способность: "
              << Q << std::endl;
    std::cout << "Абсолютная пропускная способность: "
              << A << std::endl << std::endl;

    // Simulating market
    Market m(queueLength,
             clientIntensity,
             cashboxesCount,
             timeToProcessProduct,
             avgProductsInCart);

    m.EnableLogging();
    m.Simulate(clientsCount);

    // Output simulation results
    std::cout << "Всего заявок: " << clientsCount << std::endl;
    std::cout << "Обработано заявок: " << m.ServedClients << std::endl;
    std::cout << "Отклонено заявок: " << m.RejectedClients << std::endl;
    std::cout << "Вероятность отказа: " << static_cast<double>(m.RejectedClients) / clientsCount << std::endl;
    std::cout << "Относительная пропускная способность: "
              << static_cast<double>(m.ServedClients) / clientsCount << std::endl;
    std::cout << "Абсолютная пропускная способность: "
              << static_cast<double>(m.ServedClients) / clientsCount * clientIntensity << std::endl;

    return 0;
}
