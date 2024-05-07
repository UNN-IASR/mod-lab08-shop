#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <cassert>
#include <chrono>

class Client {
  private:
  int counter;
  int product_count;
  bool was_on_cash;

  public:
  Client();
  Client(int count, int number);
  int get_count();
  bool Was_on_Cash();
};

class Cashier {
  private:
  std::thread* servicing;
  bool is_servicing;
  int serv_time;
  std::vector<Client> clients;
  std::vector<std::vector
  <std::chrono::steady_clock::time_point> > client_times;
  std::vector<double> work_times;
  std::vector<double> onhold_times;
  bool stopped = false;
 
  public:
  Cashier(int time);
  Cashier();
  void spawn();

  void client_add(Client cl);
  int queue_size();
  bool is_serv();
  std::vector<double> Work_Times();
  std::vector<double> Hold_Times();
  std::vector<Client> getClients();
  void hold_push(double val);
  void work_push(double val);
  void popfront_clients();
  void serv_beg();
  void serv_end();
  int _serv_time();
  std::vector<double> durations_client();
  void cltimes_addend(std::chrono::steady_clock::time_point
  p2, int num);
  void stop();
  bool is_stopped();
};

int CashChoose(Cashier casses[], int cash_count, int max_queue);

void service(Cashier& cash);