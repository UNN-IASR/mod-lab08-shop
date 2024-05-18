#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include "task.h"
using namespace std;
long long factorial(long long n) {
    long long fact = 1;
    for (long long i = 2; i <= n; ++i) {
        fact *= i;
    }
    return fact;
}
double P0(int count_kass, double ro, int max_dlina_ocheredi) {
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
double Pn(int count_kass, double ro, double p0, int max_dlina_ocheredi) {
    double Pn = pow(r, pool_count + max_length) / (factorial(pool_count) * pow(pool_count, max_length)) * P0;
    return Pn;
};
int main() {
    int count_kass = 3;
    int intensivnost_potoka_pokupatelei = 10;
    int skorost_obrabotki_tovara = 5;
    int srednee_kol_productov = 10;
    int max_dlina_ocheredi = 5;
    Supermarket supermarket(count_kass, intensivnost_potoka_pokupatelei, skorost_obrabotki_tovara, srednee_kol_productov, max_dlina_ocheredi);
    supermarket.work();
    supermarket.statistika();
    double ro = intensivnost_potoka_pokupatelei * srednee_kol_productov / skorost_obrabotki_tovara;
    double p0 = P0(count_kass, ro, max_dlina_ocheredi);
    double padenie = Pn(count_kass, ro, p0, max_dlina_ocheredi);
    std::cout << "veroyatnost otkaza formula " << padenie << endl;
    std::cout << "Otnositelnaya propusknaya sposobnost " << 1 - padenie << std::endl;
    std::cout << "Absolutnaya propusknaya sposobnost " << intensivnost_potoka_pokupatelei * (1-padenie) << std::endl;
    return 0;
}
