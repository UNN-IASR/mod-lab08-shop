#include <iostream>
#include <cmath>

#include "Task.h"

using namespace std;

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }

    return result;
}

int main() {
    int clientsCount = 100;
    double clientIntensity = 10;
    int queueLength = 3;
    int cashboxesCount = 7;
    int timeToProcessProduct = 100;
    double avgProductsInCart = 5;
    
    double lambda = clientIntensity;
    double mu = 1000.0 / timeToProcessProduct / avgProductsInCart;
    double rho = mu / lambda;

    double P0 = 1;
    for (int i = 1; i <= cashboxesCount; i++) {
        P0 += pow(rho, i) / factorial(i);
    }
    for(int i = 1; i <= queueLength; i++) {
        P0 += pow(rho, cashboxesCount + i) 
            / (pow(cashboxesCount, i) * factorial(i));
    }

    double P_rej = pow(rho, cashboxesCount + queueLength) * P0
                    / (pow(cashboxesCount, queueLength) * factorial(cashboxesCount));
    double Q = 1 - P_rej;
    double A = lambda * Q;

    cout << "Вероятность отказа: " << P_rej << endl;
    cout << "Относительная пропускная способность: " 
        << Q << endl;
    cout << "Абсолютная пропускная способность: " 
        << A << endl << endl;

    Market m(queueLength, 
    clientIntensity, 
    cashboxesCount, 
    timeToProcessProduct,
    avgProductsInCart);
    
    m.EnableLogging();
    m.Simulate(clientsCount);
    
    cout << "Всего заявок: " << clientsCount << endl;
    cout << "Обработано заявок: " << m.ServedClients << endl;
    cout << "Отклонено заявок: " << m.RejectedClients << endl;
    cout << "Вероятность отказа: " << (double)m.RejectedClients / clientsCount << endl;
    cout << "Относительная пропускная способность: " 
        << (double)m.ServedClients / clientsCount << endl;
    cout << "Абсолютная пропускная способность: " 
        << (double)m.ServedClients / clientsCount * clientIntensity << endl;
}