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
    cash_register_delay = static_cast<int>(1000*1.0/cash_register_intensity);
    max_queue_size = queue_size;
    processing_now = 0;
    in_queue_now = 0;
    cash_reg_statistics = std::vector<cash_reg_stat>(num);
    for (int i = 0; i < cash_reg_statistics.size(); i++) {
        cash_reg_statistics[i].is_use = false;
        cash_reg_statistics[i].downtime = 0;
        cash_reg_statistics[i].work_time = 0;
        cash_reg_statistics[i].prev_time = std::chrono::system_clock::now();
    }
}

void Shop::Processing(Buyer& buyer) {
    requestCount++;
    if (processing_now < num_of_sales_reg) {
        processing_now++;
        processedCount++;
        all_buyers.push_back(std::move(std::thread(&Shop::Work_in_thread, this,buyer)));
    }
    else if (in_queue_now < max_queue_size) {
        in_queue_now++;
        processedCount++;
        all_buyers.push_back(std::move(std::thread(&Shop::Work_in_queue_thread, this,buyer)));
    }
    else {
        std::cout << "ALL FULL!!!" << std::endl;
        rejectedCount++;
    }
    stat_queue.push_back(in_queue_now);  
}

void Shop::Work_in_thread(Buyer buyer) {
    std::unique_lock<std::mutex> lock(mtx_shop);
    int num_of_cash_reg = -1;
    for (int i = 0; i < cash_reg_statistics.size(); i++) {
        if (cash_reg_statistics[i].is_use == false) {
            cash_reg_statistics[i].is_use = true;
            num_of_cash_reg = i;
            break;
        }
    }    
    std::chrono::system_clock::time_point start = std::chrono::system_clock::now();
    cash_reg_statistics[num_of_cash_reg].downtime+=std::chrono::duration_cast<std::chrono::milliseconds>(start - cash_reg_statistics[num_of_cash_reg].prev_time).count();
    std::cout << "PROC BUYER ID:  " << buyer.Get_ID() << std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(cash_register_delay*buyer.Get_size_Cart()));
    lock.lock();
    std::cout << "BUYER: " << buyer.Get_ID() << " DONE!" << std::endl;
    processing_now--;
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    int time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    stat_time_for_buyer.push_back(time);
    cash_reg_statistics[num_of_cash_reg].prev_time = end;
    cash_reg_statistics[num_of_cash_reg].work_time+=time;
    cash_reg_statistics[num_of_cash_reg].is_use = false;
    noty.notify_one();
    lock.unlock();
}

void Shop::Work_in_queue_thread(Buyer buyer) {
    std::unique_lock<std::mutex> lock(mtx_shop);
    std::chrono::system_clock::time_point start_b = std::chrono::system_clock::now();
    noty.wait(lock, [&](){return processing_now < num_of_sales_reg;});
    std::chrono::system_clock::time_point start_c = std::chrono::system_clock::now();
    int num_of_cash_reg = -1;
    for (int i = 0; i < cash_reg_statistics.size(); i++) {
        if (cash_reg_statistics[i].is_use == false) {
            cash_reg_statistics[i].is_use = true;
            num_of_cash_reg = i;
            break;
        }
    } 
    cash_reg_statistics[num_of_cash_reg].downtime+=std::chrono::duration_cast<std::chrono::milliseconds>(start_c - cash_reg_statistics[num_of_cash_reg].prev_time).count();
    processing_now++;
    std::cout << "PROC BUYER ID:  " << buyer.Get_ID() << std::endl;
    lock.unlock();
    std::this_thread::sleep_for(std::chrono::milliseconds(cash_register_delay*buyer.Get_size_Cart()));
    lock.lock();
    std::cout << "BUYER: " << buyer.Get_ID() << " DONE!" << std::endl;
    in_queue_now--;
    processing_now--;
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    stat_time_for_buyer.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start_b).count());
    cash_reg_statistics[num_of_cash_reg].prev_time = end;
    cash_reg_statistics[num_of_cash_reg].work_time+=std::chrono::duration_cast<std::chrono::milliseconds>(end - start_c).count();
    cash_reg_statistics[num_of_cash_reg].is_use = false;
    noty.notify_one();
    lock.unlock();
}

void Shop::Join() {
    for (int i = 0; i < all_buyers.size(); i++) {
        if (all_buyers[i].joinable()) {
            all_buyers[i].join();
        }
    }
}

void Shop::Get_stat() {
    int sum_queue = 0;
    int buyers_time_sum = 0;
    int work_time = 0;
    int downtime = 0;
    for (int i = 0; i < stat_queue.size(); i++) {
        sum_queue+=stat_queue[i];
    }
    for (int i = 0; i < stat_time_for_buyer.size(); i++) {
        buyers_time_sum+=stat_time_for_buyer[i];
    }
    for (int i = 0; i < cash_reg_statistics.size(); i++) {
        work_time+=cash_reg_statistics[i].work_time;
        downtime+=cash_reg_statistics[i].downtime;
    }
    
    std::cout << "Customers have been served: " << processedCount << std::endl;
    std::cout << "Customers are not served:   " << rejectedCount << std::endl;
    std::cout << "Average queue length:       " << sum_queue*1.0/stat_queue.size() << std::endl;
    std::cout << "Average buyer service time: " << buyers_time_sum*1.0/stat_time_for_buyer.size() << std::endl;
    std::cout << "Average cash reg work time: " << work_time*1.0/num_of_sales_reg << std::endl;
    std::cout << "Average cash reg downtime:  " << downtime*1.0/num_of_sales_reg << std::endl;

}

void Shop_working::Start_sim(int num_of_buyers = 100) {
    for (int i = 0; i < num_of_buyers; i++) {
        buyers.push_back(Buyer(max_capacity_cart));
        std::cout << "BUYER ID: " << buyers.back().Get_ID() << " IS CREATED" << std::endl;
        (*shop).Processing(buyers.back());
        std::this_thread::sleep_for(std::chrono::milliseconds(buyer_delay));
    }
    (*shop).Join();
    (*shop).Get_stat();
}

Shop_working::Shop_working(int num_of_sales_reg, int buyers_intensity,
                int cash_reg_speed, int max_capacity_cart,
                int max_queue_size) {
    shop = std::make_unique<Shop>(num_of_sales_reg, cash_reg_speed, max_queue_size);
    buyer_delay = static_cast<int>(1000*1.0/buyers_intensity);
    this->max_capacity_cart = max_capacity_cart;
}
