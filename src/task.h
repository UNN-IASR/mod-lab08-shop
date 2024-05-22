#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include <string>
#include <mutex>
#include <random>
#include <queue>
#include <condition_variable>

class Shop_working;
class Buyer;
class Shop;

struct CashRegister {
    public: 
        bool in_use;
        std::thread thr;
};

class Shop {
    private:
        int cash_register_delay;
        int max_queue_size;
        int processing_now;
        int in_queue_now;
        std::mutex mtx_shop;
        std::condition_variable noty;
        std::mutex proc_mtx;
    public:
        bool is_all_cash_reg_full;
        std::vector<CashRegister> pool_of_cash_register;
        int num_of_sales_reg;
        int requestCount = 0;
        int processedCount = 0;
        int rejectedCount = 0;
        Shop() {};
        Shop (int num, int cash_register_intensity, int queue_size);
        void Processing(Buyer buyer);
        void Work_in_thread(Buyer buyer, int flag);
        void Join();
};


class Shop_working {
    private:
        std::unique_ptr<Shop> shop;
        int max_capacity_cart;
        int buyer_delay;
    public:
        Shop_working(int num_of_sales_reg, int buyers_intensity,
                     int cash_reg_speed, int max_capacity_cart,
                     int max_queue_size);
        void Start_sim(int num_of_buyers);
};


class Buyer {
    private:
        int id;
        int cap_shopping_cart;
        static inline int count_id = 0;
    public:
        Buyer() {};
        Buyer(int max_capacity_cart);
        int Get_size_Cart() {
            return cap_shopping_cart;
        }
        int Get_ID() {
            return id;
        }
};





