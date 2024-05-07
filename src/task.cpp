//Copyright Maz 2024
#include "../include/task.h"

Client::Client(int count, int number) {
  counter = number;
  product_count = count;
  was_on_cash = false;
}

Client::Client() {

}

int Client::get_count() {
  return product_count;
}

bool Client::Was_on_Cash() {
  return was_on_cash;
}

  void service(Cashier& cash) {

  Client cur;
  int servicing_client = 0;

  std::chrono::time_point start_hold =
  std::chrono::high_resolution_clock::now();
  std::chrono::time_point end_hold =
  std::chrono::high_resolution_clock::now();
  bool is_started = false;

  while (!cash.is_stopped()) {
    if(cash.getClients().size() != 0) {
      if(is_started) {
        is_started = false;
        end_hold = std::chrono::high_resolution_clock::now();
        auto duration1 = std::chrono::duration_cast
        <std::chrono::milliseconds>(end_hold - start_hold);
        double buf2 = (double)duration1.count();
        cash.hold_push(buf2);
      }

      cur = cash.getClients().at(0);
      auto start1 = std::chrono::high_resolution_clock::now();

      cash.popfront_clients();
      cash.serv_beg();//begin servicing
  
      for(int i = 1; i <= cur.get_count(); i++) {
        //servicing 1 product
        std::this_thread::sleep_for(std::chrono::milliseconds
        (cash._serv_time()));
      }
      cash.cltimes_addend(std::chrono::high_resolution_clock::now(),
      servicing_client);
      servicing_client++;

      auto stop1 = std::chrono::high_resolution_clock::now();
      auto duration = std::chrono::duration_cast
      <std::chrono::milliseconds>(stop1 - start1);
      double buf = (double)duration.count();
      cash.work_push(buf);
      //finished
      
    } else {
        if(is_started) end_hold =
        std::chrono::high_resolution_clock::now();
        else { 
          start_hold = std::chrono::high_resolution_clock::now();
          is_started = true;
        }
      }
  }
}

Cashier::Cashier(int time) {
  is_servicing=false;
  serv_time = time;
}

Cashier::Cashier() {

}

std::vector<Client> Cashier::getClients() {
  return clients;
}

void Cashier::hold_push(double val) {
  onhold_times.push_back(val);
}

void Cashier::work_push(double val) {
  work_times.push_back(val);
}

int Cashier::_serv_time() {
  return serv_time;
}

void Cashier::serv_beg() {
  is_servicing = true;
}

void Cashier::serv_end() {
  is_servicing = false;
}

void Cashier::popfront_clients() {
  assert(!clients.empty());
  clients.erase(clients.begin());
}

int Cashier::queue_size() {
  return this->clients.size();
}

bool Cashier::is_serv() {
  return this->is_servicing;
}

void Cashier::client_add(Client cl) {
  clients.push_back(cl);
  client_times.push_back({std::chrono::high_resolution_clock::now(),
  std::chrono::high_resolution_clock::now() });
}

std::vector<double> Cashier::Work_Times() {
  return work_times;
}

std::vector<double> Cashier::Hold_Times() {
  if(onhold_times.size() != 0)
  return onhold_times;
  else 
  return std::vector<double>{0};
}

int CashChoose(Cashier casses[], int cash_count, int max_queue) {
  int num = -1;
  for (int i = 0; i < cash_count; i++) {  //free ones check
    if (!(casses[i].is_serv()) &&
    !(casses[i].queue_size() > 0) && num == -1) num = i;
  }

  if (num == -1) { //minimal queue check
    int in_queue = max_queue;
    for (int i = 0; i < cash_count; i++) {  //free ones check
      if (casses[i].queue_size() < in_queue) { 
        num = i; in_queue = casses[i].queue_size(); 
      }
    }
  }
  return num;
}

void Cashier::spawn() {
  servicing =  new std::thread([this] {service(*this);});
}

std::vector<double> Cashier::durations_client() {
  std::vector<double> durations;
  for (int i = 0; i < client_times.size(); i++) {
    durations.push_back((double)std::chrono::duration_cast
    <std::chrono::milliseconds>(client_times[i][1] -
    client_times[i][0]).count());
  }
  return durations;
}

void Cashier::cltimes_addend(std::chrono::high_resolution_clock::time_point
p2, int num) {
  client_times[num] = {client_times[num][0], p2};
}

void Cashier::stop() {
  stopped = true;
}

bool Cashier::is_stopped() {
  return stopped;
}
