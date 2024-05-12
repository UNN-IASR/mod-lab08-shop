#include "task.h"

int main() {
    setlocale(LC_ALL, "Russian");
    int num_checkouts = 2;
    double lambda = 0.2;
    int checkout_time_ms = 10;
    double mean_num_items = 3;
    int max_queue_length = 5;
    task simulation(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
    simulation.startSimulation();
    double temp = 0;
    int z = 0;
    int st = 0;
    double mu = simulation.clients_served / simulation.total_time;
    double ro = lambda / mu;
    for (int i = 0; i <= num_checkouts + max_queue_length; i++) {
        temp = temp + pow(ro, i) / (pow(num_checkouts, st) * fact(z));
        if (z < num_checkouts)z++;
        else
            st++;
    }

    double P_0 = 1 / (temp * 10);
    double P_rej = pow(ro, num_checkouts + max_queue_length) * P_0 / (pow(num_checkouts, max_queue_length) * fact(num_checkouts));
    double Q = 1 - P_rej;
    double A = lambda * Q;
    double L_q = (pow(ro, num_checkouts + 1) * (1 - pow(ro / num_checkouts, max_queue_length) * (1 + max_queue_length * (1 - ro / num_checkouts))) * P_0) / (num_checkouts * fact(num_checkouts) * pow(1 - ro / num_checkouts, 2));
    double L_srv = A / mu;
    double t = L_q / lambda + Q / mu;
    double t_q = L_q / lambda;
    // Вывод статистики
    ofstream file("result.txt");
    file << "Количество обслуженных покупателей: " << simulation.clients_served << endl;
    file << "Количество отказов: " << simulation.rejected_clients << endl;
    file << "Среднее время нахождения покупателя в очереди + на кассе: " << simulation.total_time / simulation.clients_served << " ms" << endl;
    file << "Среднее время работы кассы: " << simulation.total_time / num_checkouts << " ms" << endl;
    file << "Среднее время простоя кассы: " << simulation.idle_time / num_checkouts << " ms" << endl;
    file << "Вероятность отказа: " << (double)simulation.rejected_clients / 100 << endl;
    file << "Относительная пропускная способность: " << (double)simulation.clients_served / 100 << endl;
    file << "Теоретические расчеты. " << endl;
    file << "Вероятность отказа: " << P_rej << endl;
    file << "Относительная пропускная способность: " << Q << endl;
    file << "Абсолютная пропускная способность: " << A << endl;
    file << "Среднее число заявок в очереди: " << L_q << std::endl;

    file.close();
}