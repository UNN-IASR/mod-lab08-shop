#include <deque>
#include <thread>
#include <vector>
#include <future>


class Client{
    public:
        int id;
        int product_count;
        Client(int id, int products_count);
};

class Server{
    struct Statistics
    {
        int deque_len = 0;
        int client_wait = 0;
        int customers_served = 0;
        int customers_rejected = 0;
        int thread_working = 0;
        int thread_plain = 0;
        int observations_count = 0;
    };
    class CashBox{
        int cur_client_id = -1;
        std::future<void> thrd;
        int process_time;
        void process(int product_count);
        public:
            Statistics* stats;
            CashBox(int process_time, Statistics &stats);
            bool in_use = false;
            void start_processing(Client client);
    };
    void monitoring();
    std::deque<Client> client_queue;
    int deque_max_size;
    std::vector<CashBox> cashbox_pool;
    int monitoring_interval;
    std::thread main_thread;
    std::thread monitoring_thread;
    bool is_running = false;
    public:
        void Run();
    public:
        void add_to_queue(Client client);
        Server(int threads_count, int queue_size, int process_time);
        Statistics stats;
        void Start();
        void Stop();
};
