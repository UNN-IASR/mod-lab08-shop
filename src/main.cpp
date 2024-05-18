#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "task.h"
using namespace std;

double calculateP10(int n, double rho) {
    double sum = 1;
    for (int i = 1; i <= n; i++) {
        sum += pow(rho, i) / tgamma(i + 1);
    }
    return 1 / sum;
}

double calculatePq(int n, double rho, int m) {
    return pow(rho, n) / tgamma(n + 1) * pow(1 - rho / n, m) / (1 - rho / n);
}

double calculatePrej(int n, double rho, int m) {
    return pow(rho, n + m) / tgamma(n + m + 1) * calculateP10(n, rho);
}

 //Функция для вычисления факториала
long long factorial(long long n) {
    long long fact = 1;
    for (long long i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}

// Функция для вычисления суммы ряда
double calculateSumSeries(double rho, int m, int n) {
    double sum = 0;
    for (int i = 0; i <= m + n; ++i) {
        if (i == 0) {
            sum += 1;
        }
        else {
            sum += pow(rho, i) / factorial(i);
        }
    }
    return sum;
}

// Функция для расчета вероятности P0
double calculateP01(double lambda, double mu, int m, int n) {
    double rho = lambda / (mu );
    double sum = calculateSumSeries(rho, m, n);
    return pow(sum, -1);
}
double calculate_P0(int pool_count, double r, int max_length) {
    double P0 = 0;
    for (int i = 0; i <= pool_count; i++)
    {
        P0 = P0 + pow(r, i) / factorial(i);
    }
    for (int i = 1; i <= max_length; i++)
    {
        P0 = P0 + pow(r, pool_count + i) / (factorial(pool_count) * pow(pool_count, i));
    }
    return pow(P0, -1);
};
double calculate_Pn(int pool_count, double r, double P0, int max_length) {
    double Pn = pow(r, pool_count + max_length) / (factorial(pool_count) * pow(pool_count, max_length)) * P0;
    return Pn;
};
int main() {

    cout << "kjkjkj" << std::endl;
    // Параметры модели
    int count_kass = 3;
    int intensivnost_potoka_pokupatelei = 10;
    int skorost_obrabotki_tovara = 5;
    int srednee_kol_productov = 10;
    int max_dlina_ocheredi = 5;
    //Supermarket supermarket1(count_kass);
    // Создание супермаркета с заданными параметрами
    Supermarket supermarket(count_kass, intensivnost_potoka_pokupatelei, skorost_obrabotki_tovara, srednee_kol_productov, max_dlina_ocheredi);

    // Запуск работы супермаркета
    supermarket.work();

    // Вывод статистики
    supermarket.statistika();
    cout << "kjkjkj" << endl;
    double t = (double)srednee_kol_productov / skorost_obrabotki_tovara;
    double mu = (double)1 / t;
    
    double rr0 = (double)intensivnost_potoka_pokupatelei / mu;
    double prej = (double)(pow(rr0, count_kass + supermarket.srednya_dlina_ocheredi) / (pow(count_kass, supermarket.srednya_dlina_ocheredi) * factorial(count_kass)));// *p0;
    
    
    double r = intensivnost_potoka_pokupatelei * srednee_kol_productov / skorost_obrabotki_tovara;
    double p0 = calculate_P0(count_kass, r, max_dlina_ocheredi);
    double failureProbability = calculate_Pn(count_kass, r, p0, max_dlina_ocheredi);
    std::cout << "veroyatnost otkaza formula " << failureProbability << endl;
    std::cout << "Otnositelnaya propusknaya sposobnost " << 1 - failureProbability << std::endl;
    std::cout << "Absolutnaya propusknaya sposobnost " << intensivnost_potoka_pokupatelei * (1-failureProbability) << std::endl;

	return 0;


}