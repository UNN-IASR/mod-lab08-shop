#include "../include/task.h"
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
    double mu = 1000.0 / processingSpeedProducts / avgProductsInCart; // ñòðåäíåå âðåìÿ îáñëóæèâàíèÿ êëèåíòà
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
    cout << "Âåðîÿòíîñòü îòêàçà: " << P_rej << endl;
    cout << "Îòíîñèòåëüíàÿ ïðîïóñêíàÿ ñïîñîáíîñòü: "
        << Q << endl;
    cout << "Àáñîëþòíàÿ ïðîïóñêíàÿ ñïîñîáíîñòü: "
        << A << endl << endl;
    Shop s(maxQueue,
        clientIntensity,
        numberCash,
        processingSpeedProducts,
        avgProductsInCart);
    s.EnableLogging();
    s.Simulate(clientsCount);
    cout << "Âñåãî çàÿâîê: " << clientsCount << endl;
    cout << "Îáðàáîòàíî çàÿâîê: " << s.ServedClients << endl;
    cout << "Îòêëîíåíî çàÿâîê: " << s.RejectedClients << endl;
    cout << "Âåðîÿòíîñòü îòêàçà: " << (double)s.RejectedClients / clientsCount << endl;
    cout << "Îòíîñèòåëüíàÿ ïðîïóñêíàÿ ñïîñîáíîñòü: "
        << (double)s.ServedClients / clientsCount << endl;
    cout << "Àáñîëþòíàÿ ïðîïóñêíàÿ ñïîñîáíîñòü: "
        << (double)s.ServedClients / clientsCount * clientIntensity << endl;
    return 0;
}
