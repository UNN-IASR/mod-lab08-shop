#pragma once
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>
class Client {
 public:
  Client(int items);
  int num_items;
};
class task {
 public:
  task(int num_checkouts, double lambda, int checkout_time_ms,
       int max_queue_length, int mean_num_items);
  void startSimulation();
  int clients_served = 0;
  int rejected_clients = 0;
  double total_time = 0.0;
  double idle_time = 0.0;
  std::vector<std::thread> checkout_threads;
  // int max_queue_length;

 private:
  int num_checkouts;  // Количество касс
  double lambda;  // Интенсивность потока покупателей
  int checkout_time_ms;  // Скорость обработки товара на кассе (в миллисекундах)
  double mean_num_items;  // Среднее количество товаров в тележке покупателя
  int max_queue_length;   // Максимальная длина очереди
  std::queue<Client> client_queue;

  std::mutex queue_mutex;
  std::condition_variable queue_condition;
  std::mutex idle_mutex;
  std::condition_variable idle_condition;
  std::queue<int> idle_checkouts;
  std::vector<int> checkout_times;
  bool clients_finished;
  void serveClients(int id);
  void generateClients();
};
