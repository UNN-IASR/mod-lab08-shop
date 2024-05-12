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
    // ����� ����������
    ofstream file("result.txt");
    file << "���������� ����������� �����������: " << simulation.clients_served << endl;
    file << "���������� �������: " << simulation.rejected_clients << endl;
    file << "������� ����� ���������� ���������� � ������� + �� �����: " << simulation.total_time / simulation.clients_served << " ms" << endl;
    file << "������� ����� ������ �����: " << simulation.total_time / num_checkouts << " ms" << endl;
    file << "������� ����� ������� �����: " << simulation.idle_time / num_checkouts << " ms" << endl;
    file << "����������� ������: " << (double)simulation.rejected_clients / 100 << endl;
    file << "������������� ���������� �����������: " << (double)simulation.clients_served / 100 << endl;
    file << "������������� �������. " << endl;
    file << "����������� ������: " << P_rej << endl;
    file << "������������� ���������� �����������: " << Q << endl;
    file << "���������� ���������� �����������: " << A << endl;
    file << "������� ����� ������ � �������: " << L_q << std::endl;

    file.close();
}