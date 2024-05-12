#include "task.h"

int factorial(int n) {
    int res = 1;
    for (int i = 2; i <= n; i++) res *= i;
    return res;
}

int main() {
    int cashNum = 4;
    double clientFlow = 10.0;
    double cashSpeed = 5.0;
    int queueMax = 10;
    int cartSize = 5;

    Store sto = Store(cashNum, cashSpeed, queueMax);
    ClientGenerator cli = ClientGenerator(&sto, cartSize);
    for (int id = 1; id <= 200; id++) {
        cli.Send(id);
        auto delay = std::chrono::milliseconds((int)(1000.0 / clientFlow));
        std::this_thread::sleep_for(delay);
    }
    sto.Close();
    sto.Stats();
    
    std::cout << "Статистика" << std::endl;
    std::cout << "Принято клиентов " << sto.accepted << std::endl
              << "Отклонено клиентов " << sto.rejected << std::endl
              << "Всего клиентов " << sto.total << std::endl
              << "Среднее количество клиентов в очереди " << sto.avgQueue << std::endl
              << "Среднее время нахождения клиента в очереди " << sto.avgInQueue << std::endl
              << "Среднее время нахождения клиента на кассе " << sto.avgInCash << std::endl
              << "Среднее время работы кассы " << sto.avgCashWork << std::endl;
    std::cout << "Вывод" << std::endl;
    std::cout << "Вероятность отказа " << (double)sto.rejected / sto.total << std::endl 
              << "Относительная пропускная " << (double)sto.accepted / sto.total << std::endl
              << "Абсолютная пропускная " << (double)sto.accepted / sto.total * clientFlow << std::endl;


    int n = cashNum;
    int m = queueMax;
    double lambda = clientFlow;
    double mu = cashSpeed / cartSize;
    double ro = lambda / mu;
    double P0 = 0;
    for (int k = 0; k <= n; k++) P0 += pow(ro, k) / factorial(k);
    for (int k = 1; k <= m; k++) P0 += pow(ro, n+k) / (pow(n, k) * factorial(n));
    P0 = pow(P0, -1);
    double Pq = (pow(ro, n)/factorial(n)) * ((1-pow(ro/n, m)) / (1-(ro/n))) * P0;
    double Prej = P0 * pow(ro, n+m) / (pow(n, m) * factorial(n));
    double Q = 1 - Prej;
    double A = lambda * Q;

    std::cout << "Формулы" << std::endl;
    std::cout << "Вероятность отказа " << Prej << std::endl 
              << "Относительная пропускная " << Q << std::endl
              << "Абсолютная пропускная " << A << std::endl;
}