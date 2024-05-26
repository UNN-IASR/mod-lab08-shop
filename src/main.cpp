#include "task.h"
#include <fstream>
#include <math.h>

int fac(int n) {
    if (n == 0)
        return 1;
    else {
        int result = 1;
        for (int i = 1; i <= n; i++)
            result = result * i;
        return result;
    }
}

int main() {
    setlocale(0, "rus");
    int num_checkouts = 2;
    double lambda = 0.2;
    int checkout_time_ms =10;
    double mean_num_items = 3; 
    int max_queue_length =5; 
    task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
    s.startSimulation();
    double temp = 0;
    int z = 0;
    int st = 0;
    double mu = s.clients_served / s.total_time;
    double ro = lambda / mu;
    for (int i = 0; i <= num_checkouts + max_queue_length; i++) {
        temp =temp+ pow(ro, i) / (pow(num_checkouts, st) * fac(z));
        if (z < num_checkouts)z++;
        else
            st++;
    }
    
    double P_0 = 1 / (temp*10);
    double P_rej = pow(ro, num_checkouts + max_queue_length) * P_0 / (pow(num_checkouts, max_queue_length) * fac(num_checkouts));
    double Q = 1 - P_rej;
    double A = lambda * Q;
    double L_q = (pow(ro, num_checkouts + 1) * (1 - pow(ro / num_checkouts, max_queue_length) * (1 + max_queue_length * (1 - ro / num_checkouts))) * P_0) / (num_checkouts * fac(num_checkouts) * pow(1 - ro / num_checkouts, 2));
    double L_srv = A / mu;
    double t = L_q / lambda + Q / mu;
    double t_q = L_q / lambda;
    // Вывод статистики
    std::ofstream file("result.txt");
    file << "Количество обслуженных покупателей: " << s.clients_served << std::endl;
    file << "Количество отказов: " << s.rejected_clients << std::endl;
    file << "Среднее время нахождения покупателя в очереди + на кассе: " << s.total_time / s.clients_served << " ms" << std::endl;
    file << "Среднее время работы кассы: " << s.total_time / num_checkouts << " ms" << std::endl;
    file << "Среднее время простоя кассы: " << s.idle_time / num_checkouts << " ms" << std::endl;
    file << "Вероятность отказа: " << (double)s.rejected_clients/100 << std::endl;
    file << "Относительная пропускная способность: " << (double)s.clients_served/100 << std::endl;
    file << "Теоретические расчеты. " << std::endl;

    file << "Вероятность отказа: " << P_rej << std::endl;
    file << "Относительная пропускная способность: " << Q << std::endl;
    file << "Абсолютная пропускная способность: " << A << std::endl;
    file << "Среднее число заявок в очереди: " << L_q << std::endl;
    

    
    file.close();
    
}
