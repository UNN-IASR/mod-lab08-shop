#pragma once
#include <list>
#include <vector>
#include <queue>
#include <mutex>
#include <random>
#include <thread>
#include <condition_variable>
#include <iostream>

class Buyer {
public:
  std::vector<int> products;
  int number_of_items;
  explicit Buyer(std::vector<int> items, int number);
};

class Supermarket {
private:
  std::vector<std::thread*> checkout_threads;
  std::queue<Buyer*> customer_queue;
  std::vector<std::queue<Buyer*>*> register_queues;

  int register_count; 
  int flow_rate; 
  int processing_speed; 
  int avg_items_per_customer; 
  int max_queue_length; 

  int max_price = 50;
  int max_customers;
  int customer_count = 0; 

  int served_customers = 0;  
  double avg_queue_length = 0;  
  double service_time = 0;  
  double avg_checkout_working_time;  
  double avg_checkout_idle_time;  

  int working_checkout_count = 1;
  int rejected_customers = 0;

  std::mutex market_mutex;
public:
  Supermarket(int registers, int flow_rate, int processing_speed, int avg_items, int max_length, int max_customers);
  Buyer* create_customer(int number);
  void run();
  void serve_customer(Buyer* customer, int number);
  void serve_queue(std::queue <Buyer*>* customers);
  void serve_market();

  int get_served_customers();
  int get_unserved_customers();
  double get_avg_queue_length();
  double get_service_time();
  double get_avg_checkout_working_time();
  double get_avg_checkout_idle_time();
  double rejection_probability();
  double relative_throughput();
  double absolute_throughput();
  double factorial(double i);
};
