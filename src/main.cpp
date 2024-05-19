#include "task.h"
#include <iostream>

using namespace std;

int factorial(int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main()
{
    setlocale(LC_ALL, "Rus");
    int clientsCount = 100;
    double numberCash = 5;
    double clientIntensity = 20;
    double processingSpeedProducts = 100;
    double avgProductsInCart = 5;
    double maxQueue = 5;
    double lambda = clientIntensity;
    double mu = 1000.0 / processingSpeedProducts / avgProductsInCart; // стреднее время обслуживания клиента
    double p = mu / lambda;
    double P0 = 1;
    for (int i = 1; i <= numberCash; i++)
    {
        P0 += pow(p, i) / factorial(i);
    }
    for (int i = 1; i <= maxQueue; i++) {
        P0 += pow(p, numberCash + i)
            / (pow(numberCash, i) * factorial(i));
    }
    double P_rej = pow(p, numberCash + maxQueue) * P0
        / (pow(numberCash, maxQueue) * factorial(numberCash));
    double Q = 1 - P_rej;
    double A = lambda * Q;
    cout << "Вероятность отказа: " << P_rej << endl;
    cout << "Относительная пропускная способность: "
        << Q << endl;
    cout << "Абсолютная пропускная способность: "
        << A << endl << endl;
    Shop s(maxQueue,
        clientIntensity,
        numberCash,
        processingSpeedProducts,
        avgProductsInCart);
    s.EnableLogging();
    s.Simulate(clientsCount);
    cout << "Всего заявок: " << clientsCount << endl;
    cout << "Обработано заявок: " << s.ServedClients << endl;
    cout << "Отклонено заявок: " << s.RejectedClients << endl;
    cout << "Вероятность отказа: " << (double)s.RejectedClients / clientsCount << endl;
    cout << "Относительная пропускная способность: "
        << (double)s.ServedClients / clientsCount << endl;
    cout << "Абсолютная пропускная способность: "
        << (double)s.ServedClients / clientsCount * clientIntensity << endl;
    return 0;
}