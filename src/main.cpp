#include "../include/task.h"

int factorial(int n) {
    return n < 2 ? 1 : n * factorial(n-1);
}

int main() {
    int pool_size = 3;
    int queue_len = 7;
    int client_flow = 10; // per sec
    int cash_speed = 100; // msec for item
    int cart_size = 5;
    int clients_count = 1000;

    int n = pool_size;
    int m = queue_len;
    double lambda = client_flow;
    double mu = 1000.0 / cash_speed / cart_size;
    double ro = lambda / mu;

    double P0 = 0;
    for (int k = 0; k <= n; k++) P0 += pow(ro, k) / factorial(k);
    for (int k = 1; k <= m; k++) P0 += pow(ro, n+k) / (pow(n, k) * factorial(n));
    P0 = pow(P0, -1);
    double Pq = (pow(ro, n)/factorial(n)) * ((1-pow(ro/n, m)) / (1-(ro/n))) * P0;
    double Prej = P0 * pow(ro, n+m) / (pow(n, m) * factorial(n));
    double Q = 1 - Prej;
    double A = lambda * Q;
    double Lq = (pow(ro, n+1)/(n*factorial(n))) * (1-pow(ro/n, m)*(1+m*(1-ro/n))) / pow(1-ro/n, 2) * P0;
    double Lsrv = A / mu;
    double t = Lq/lambda + Q/mu;
    double tq = Lq/lambda;

    std::cout << "expected ~" << clients_count << " clients" 
            << "\nreject prob: " << Prej
            << "\nrel throughput: " << Q
            << "\nabs throughput: " << A
            << "\navg clients in queue: " << Lq
            << "\navg client waiting time: " << t
            << "\navg client serving time: " << tq
            << "\navg serving clients at moment: " << Lsrv
            << "\nextra stats:"
            << "\nsystem load: " << ro
            << "\nsystem idle prob: " << P0
            << "\nqueue prob: " << Pq
            << std::endl << std::endl;

    market mark {
        pool_size, 
        client_flow,
        cash_speed,
        cart_size,
        queue_len
    };
    mark.run(clients_count);
    stats s = mark.get_stats();
    std::cout << "accepted clients: " << s.accepted_clients
            << "\nrejected clients: " << s.rejected_clients
            << "\nreject prob: " << s.reject_prob
            << "\nrel throughput: " << s.rel_throughput
            << "\nabs throughput: " << s.abs_throughput
            << "\navg clients in queue: " << s.avg_queue_len
            << "\navg client waiting time: " << s.avg_client_queue / 1000
            << "\navg client serving time: " << s.avg_client_cash / 1000
            << "\navg serving clients at moment: " << s.avg_clients_at_moment
            << "\nextra stats:"
            << "\nmarket worktime: " << s.market_worktime / 1000
            << "\navg cash worktime: " << s.avg_cash_worktime / 1000
            << "\navg cash idletime: " << s.avg_cash_idletime / 1000
            << "\nprofit: " << s.profit
            << "\nloss: " << s.loss
            << std::endl; 
}