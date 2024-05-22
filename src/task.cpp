#include "task.h"

Buyer::Buyer(int max_capacity_cart) {
    count_id++;
    id = count_id;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, max_capacity_cart);
    cap_shopping_cart = dist(gen);
}



Shop::Shop (int num, int cash_register_intensity, int queue_size) {
    num_of_sales_reg = num;
    pool_of_cash_register = std::vector<CashRegister>(num_of_sales_reg);
    cash_register_delay = static_cast<int>(1000*1.0/cash_register_intensity);
    max_queue_size = queue_size;
    processing_now = 0;
    in_queue_now = 0;
    is_all_cash_reg_full = false;
}

/* void Shop::Processing(Buyer buyer) {
    if (processing_now < num_of_sales_reg) {
        for (int i = 0; i < num_of_sales_reg; i++) {
            if (pool_of_cash_register[i].in_use == false) {
                pool_of_cash_register[i].in_use = true;
                if (pool_of_cash_register[i].thr.joinable()) pool_of_cash_register[i].thr.join();
                pool_of_cash_register[i].thr = std::thread(&Shop::Work_in_thread, this, buyer, i);
                processing_now++;
                break;
            }
        }
    }
    else if (in_queue_now < max_queue_size) {

    }
    else {

    }
} */

void Shop::Processing(Buyer buyer) {
    if (processing_now < num_of_sales_reg) {
        processing_now++;
        std::thread th(&Shop::Work_in_thread, this, buyer, 0);
    }
    else if (in_queue_now < max_queue_size) {
        in_queue_now++;
        std::thread th(&Shop::Work_in_thread, this, buyer, 1);
    }
    else {

    }
}

/* void Shop::Work_in_thread(Buyer buyer, int flag) {
    std::unique_lock<std::mutex> lock(mtx_shop);
    std::cout << "PROC BUYER ID:  " << buyer.Get_ID() << std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    lock.lock();
    std::cout << "BUYER: " << buyer.Get_ID() << " DONE!" << std::endl;
    pool_of_cash_register[id].in_use = false;
    processing_now--;
    lock.unlock();
} */

void Shop::Work_in_thread(Buyer buyer, int flag) {
    std::unique_lock<std::mutex> lock(mtx_shop);
    std::cout << "PROC BUYER ID:  " << buyer.Get_ID() << std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(5000));
    lock.lock();
    std::cout << "BUYER: " << buyer.Get_ID() << " DONE!" << std::endl;
    if (flag == 0) {
        processing_now--;
    }
    else {
        in_queue_now--;
    }
    lock.unlock();
}
 
void Shop::Join() {
    for (int i = 0; i < num_of_sales_reg; i++) {
        if (pool_of_cash_register[i].thr.joinable()) {
            pool_of_cash_register[i].thr.join();
        }
    }
}

void Shop_working::Start_sim(int num_of_buyers = 100) {
    Buyer new_buyer;
    for (int i = 0; i < num_of_buyers; i++) {
        new_buyer = Buyer(max_capacity_cart);
        std::cout << "BUYER ID: " << new_buyer.Get_ID() << " IS CREATED" << std::endl;
        (*shop).Processing(new_buyer);
        std::this_thread::sleep_for(std::chrono::milliseconds(buyer_delay));
    }
    (*shop).Join();

}

Shop_working::Shop_working(int num_of_sales_reg, int buyers_intensity,
                int cash_reg_speed, int max_capacity_cart,
                int max_queue_size) {
    shop = std::make_unique<Shop>(num_of_sales_reg, cash_reg_speed, max_queue_size);
    buyer_delay = static_cast<int>(1000*1.0/buyers_intensity);
    this->max_capacity_cart = max_capacity_cart;
}

//здесь был Вася