#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>

using time_point = std::chrono::system_clock::time_point;

struct client {
    int id;
    std::vector<int> cart;
    time_point in_queue;
};

struct stats {
    int accepted_clients;
    int rejected_clients;
    double reject_prob;
    double rel_throughput;
    double abs_throughput;
    double avg_queue_len;
    double avg_client_queue;
    double avg_client_cash;
    double avg_clients_at_moment;
    int market_worktime;
    int profit;
    int loss;
    double avg_cash_worktime;
    double avg_cash_idletime;
};

class market {
 private:
    std::vector< std::thread > pool;
    std::queue<client> queue;
    int pool_size;
    int client_flow;
    int cash_speed;
    int cart_size;
    int queue_len;

    bool opened;
    int accepted_clients = 0;
    int rejected_clients = 0;
    int clients_count = 0;
    int positive = 0;
    int negative = 0;

    double avg_queue_len = 0;
    double avg_busy_cash = 0;
    double avg_idle_cash = 0;
    double avg_client_queue = 0;
    double avg_client_cash = 0;
    int market_time = 0;

    std::mutex mtx;
    std::random_device rd;
    std::mt19937 mt;
    std::poisson_distribution<int> p_flow;
    std::poisson_distribution<int> p_cart;
    std::uniform_int_distribution<int> idist;
 private:
    client create_client(int id, int cart_size);
    int proc_client();
    void cash_reg();
 public:
    market(int pool_size, int client_flow, int cash_speed, int cart_size, int queue_len);
    void run(int clients_count);
    stats get_stats() const;
};