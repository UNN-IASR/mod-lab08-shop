#include <task.h>
#include <iostream>

Server::Server(int threads_count, int queue_size, int process_time){
    cashbox_pool = std::vector<CashBox>();
    for(int i = 0; i < threads_count; i++){
        cashbox_pool.push_back(CashBox(process_time, stats));
    }
    stats.client_wait = 0;
    stats.thread_plain = 0;
    stats.thread_working = 0;
    client_queue = std::deque<Client>();
    deque_max_size = queue_size;
}

void Server::Stop(){
    is_running = false;
    stats.customers_rejected += client_queue.size();
    monitoring_thread.join();
    main_thread.join();
}

void Server::monitoring(){
    while(is_running){
        for(int i = 0; i < cashbox_pool.size(); i++){
            if (cashbox_pool[i].in_use){
                stats.thread_working += monitoring_interval;
            }
            else{
                stats.thread_plain += monitoring_interval; 
            }
        }
        stats.client_wait += client_queue.size() * monitoring_interval;
        stats.deque_len += client_queue.size();
        stats.observations_count ++;
        std::this_thread::sleep_for(std::chrono::milliseconds(monitoring_interval));
    }
}

void Server::Run(){
    while (is_running)
    {
        for(int i = 0; i < cashbox_pool.size(); i++){
            if(!cashbox_pool[i].in_use && client_queue.size() > 0){
                cashbox_pool[i].start_processing(client_queue.front());
                client_queue.pop_front();
            }
        }
    }
}

void Server::Start(){
    is_running = true;
    monitoring_thread = std::thread(&Server::monitoring, this);
    main_thread = std::thread(&Server::Run, this);
}

void Server::add_to_queue(Client client){
    if(deque_max_size > client_queue.size()){
        client_queue.push_back(client);
    }
    else {
        stats.customers_rejected ++;
    }
}

Server::CashBox::CashBox(int process_time, Statistics &stats){
    this->stats = &stats;
    in_use = false;
    this->process_time = process_time;
}

void Server::CashBox::start_processing(Client client){
    this->in_use = true;
    cur_client_id = client.id;
    thrd = std::async(&Server::CashBox::process, this, client.product_count);
}

void Server::CashBox::process(int product_count){
    for(int i = 0; i < product_count; i++){
        std::this_thread::sleep_for(std::chrono::milliseconds(this->process_time));
    }
    //ToDo: thread lock on stats recording maybe (mutex?)
    this->stats->client_wait += product_count * this->process_time;
    this->stats->customers_served ++;
    this->in_use = false;
}

Client::Client(int id, int products_count){
    this->id = id;
    this->product_count = products_count;
}
