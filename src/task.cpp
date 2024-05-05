#include "../include/task.h"

market::market(int _pool_size, int _client_flow, int _cash_speed, int _cart_size, int _queue_len) :
    mt(rd()), p_flow(_client_flow), p_cart(_cart_size), idist(10, 60)
{
    pool_size = _pool_size;
    client_flow = _client_flow;
    cash_speed = _cash_speed;
    cart_size = _cart_size;
    queue_len = _queue_len;
}

client market::create_client(int id, int cart_size) {
    std::vector<int> cart(p_cart(mt));
    auto gen = [&] () { return idist(mt); };
    std::generate(cart.begin(), cart.end(), gen);
    return { id, cart, std::chrono::system_clock::now() };
}

int market::proc_client() {
    std::unique_lock<std::mutex> lock(mtx);
    if (queue.empty()) return 0;
    client c = queue.front();
    time_point now = std::chrono::system_clock::now();
    avg_client_queue += std::chrono::duration_cast<std::chrono::milliseconds>(now - c.in_queue).count();
    queue.pop();
    accepted_clients++;
    int proc_time = cash_speed * c.cart.size();
    avg_busy_cash += proc_time;
    avg_client_cash += proc_time;
    return proc_time;
}

void market::cash_reg() {
    while (opened || !queue.empty()) {
        int proc_time = proc_client();
        auto delay = std::chrono::milliseconds(proc_time);
        std::this_thread::sleep_for(delay);
    }
}

void market::run(int _clients_count) {
    // prepare clients
    std::vector<int> times;
    while (times.size() < _clients_count) {
        int r = p_flow(mt);
        int t = 1000 / (r == 0 ? 1 : r);
        for (int i = 0; i < r; i++)
            times.push_back(t);
    }
    std::shuffle(times.begin(), times.end(), rd);
    clients_count = times.size();
    // open market
    opened = true;
    time_point start = std::chrono::system_clock::now();
    for (int _ = 0; _ < pool_size; _++) {
        std::thread t(&market::cash_reg, this);
        pool.push_back(std::move(t));
    }
    // proceed clients
    for (int id = 0; id < times.size(); id++) {
        client c = create_client(id, cart_size);
        if (queue.size() < queue_len) {
            queue.push(c);
            positive += std::accumulate(c.cart.begin(), c.cart.end(), 0);
        } else {
            rejected_clients++;
            negative += std::accumulate(c.cart.begin(), c.cart.end(), 0);
        }
        avg_queue_len += queue.size();
        auto delay = std::chrono::milliseconds(times[id]);
        std::this_thread::sleep_for(delay);
    }
    // close market
    opened = false;
    for (int i = 0; i < pool_size; i++) {
        pool[i].join();
    }
    time_point end = std::chrono::system_clock::now();
    market_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}

stats market::get_stats() const {
    return {
        accepted_clients,
        rejected_clients,
        (double)rejected_clients / clients_count,
        (double)accepted_clients / clients_count,
        (double)accepted_clients / clients_count * client_flow,
        avg_queue_len / clients_count,
        avg_client_queue / accepted_clients,
        avg_client_cash / accepted_clients,
        avg_busy_cash / market_time,
        market_time,
        positive,
        negative,
        avg_busy_cash / pool_size,
        market_time - avg_busy_cash / pool_size
    };
}