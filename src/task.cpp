#include "task.h"

Buyer::Buyer(std::vector<int> items, int number) {
  products = items;
  number_of_items = number;
}

Supermarket::Supermarket(int registers, int flow_rate, int processing_speed, int avg_items, int max_length, int max_customers) {
  register_count = registers;
  flow_rate = flow_rate;
  processing_speed = processing_speed;
  avg_items_per_customer = avg_items;
  max_queue_length = max_length;
  max_customers = max_customers;
}

Buyer* Supermarket::create_customer(int number) {
  int count_products = std::rand() % (avg_items_per_customer + 5) + (avg_items_per_customer - 5);
  std::vector<int> items(count_products);
  for (int i = 0; i < count_products; i++) {
    items[i] = std::rand() % max_price + 1;
  }
  std::cout << "New customer" << '\n';
  return new Buyer(items, number);
}

void Supermarket::run() {
  serve_market();
  for (auto currQueue : checkout_threads) {
    currQueue->join();
  }
}

void Supermarket::serve_customer(Buyer* customer, int number) {
  for (auto i = 0; i < customer->products.size(); i++) {
    int time = std::rand() % processing_speed;
    std::this_thread::sleep_for(std::chrono::milliseconds(time));
    std::unique_lock<std::mutex> unqLock(market_mutex);
    std::cout << "Checkout " << std::this_thread::get_id() << ": customer " << number << " gets product " << i + 1 << '\n';
    unqLock.unlock();
    service_time += time;
    avg_checkout_working_time += processing_speed / working_checkout_count;
    avg_checkout_idle_time += processing_speed / (register_count - working_checkout_count);
  }
  served_customers++;
}

void Supermarket::serve_queue(std::queue <Buyer*>* customers) {
  auto customer = customers->front();
  customers->pop();
  serve_customer(customer, customer->number_of_items);

  while (!customer_queue.empty()) {
    std::unique_lock<std::mutex> unqLock(market_mutex);
    customers->push(customer_queue.front());
    customer_queue.pop();
    unqLock.unlock();

    auto customer = customers->front();
    customers->pop();
    serve_customer(customer, customer->number_of_items);
  }
}

void Supermarket::serve_market() {
  for (int i = 0; i < max_customers; i++) {
    customer_count++;
    if (working_checkout_count <= register_count) {
      auto ptrQ = new std::queue<Buyer*>;
      ptrQ->push(create_customer(customer_count));
      register_queues.push_back(ptrQ);
      checkout_threads.push_back(new std::thread(&Supermarket::serve_queue, this, ptrQ));
      working_checkout_count++;
    }

    else if (customer_queue.size() < max_queue_length) {
      customer_queue.push(create_customer(customer_count));
    }

    avg_queue_length += customer_queue.size();

  }
  avg_queue_length = avg_queue_length / customer_count;
  avg_checkout_working_time = avg_checkout_working_time / customer_count;
  avg_checkout_idle_time = avg_checkout_idle_time / customer_count;

  service_time = service_time / customer_count;
}

int Supermarket::get_served_customers() {
  return served_customers;
}

int Supermarket::get_unserved_customers() {
  return customer_count - served_customers;
}

double Supermarket::get_avg_queue_length() {
  return avg_queue_length;
}

double Supermarket::get_service_time() {
  return service_time;
}

double Supermarket::get_avg_checkout_working_time() {
  return avg_checkout_working_time;
}

double Supermarket::get_avg_checkout_idle_time() {
  return avg_checkout_idle_time;
}

double Supermarket::rejection_probability() {
  return (rejected_customers / customer_count);


  double p = static_cast<double>(flow_rate) / (processing_speed);

  double P0 = 1.0;
  for (int i = 1; i <= register_count; i++) {
    P0 = P0 + pow(p, i) / factorial(i);
  }

  for (int i = register_count + 1; i < register_count + max_queue_length; i++) {
    P0 = P0 + pow(p, i) / (factorial(register_count) * pow(register_count, i - register_count));
  }

  P0 = 1.0 / P0;

  double Prej = P0 * pow(p, (register_count + max_queue_length)) / static_cast<double>(pow(register_count, max_queue_length) * factorial(register_count));
  return Prej;
}

double Supermarket::relative_throughput() {
  return 1 - rejection_probability();
}

double Supermarket::absolute_throughput() {
  return relative_throughput() * processing_speed;
}

double Supermarket::factorial(double i) {
  if (i == 0) return 1;
  else return i * factorial(i - 1);
}
