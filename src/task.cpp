#include "task.h";


int Random_number(int a, int b) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(a, b);
    return dist(gen);
}

static int Factorial(int n) {
    int answer = 1;
    for (int i = 1; i <= n; i++) {
        answer *= i;
    }
    return answer;
}

Shop::Shop(int c, int i, int s, int p, int l) {
    this->count = c;
    this->intensity = i;
    this->speed = s;
    this->av_product = p;
    this->max_length = l;
}

std::shared_ptr<Shop::Customer> Shop::Gen_customer() {
    int prodCount = av_product + Random_number(1, av_product);
    std::vector<int> list;
    list.resize(prodCount);
    for (int i = 0; i < prodCount; ++i) {
        list[i] = Random_number(1, 50);
    }
    return std::make_shared<Customer>(list);
}

void Shop::Run() {
    int sum = 0;
    for (int i = 0; i < max_customer; ++i) {
        bool remain = false;
        for (auto it = begin(queue_B);
            it != end(queue_B); ++it) {
            sum += (*it)->size();
        }
        av_lenght = (av_lenght * i + sum) / double(i + 1);
        for (auto it = begin(queue_B);
            it != end(queue_B); ++it) {
            std::this_thread::sleep_for(std::chrono::milliseconds(intensity));
            if ((*it)->size() < max_length) {
                (*it)->push(Gen_customer());
                remain = true;
                break;
            }
        }
        if (!remain && queue_B.size() < count) {
            queueCust q =
                std::make_shared<std::queue<std::shared_ptr<Customer>>>();
            q->push(Gen_customer());
            queue_B.push_back(q);
            threads.push_back(std::thread(&Shop::Serv_Q, this, q));
        }
    }
    for (int i = 0; i < threads.size(); ++i) {
        threads[i].join();
    }
}

void Shop::Serv_customers(Customer& cust) {
    for (int i = 0; i < cust.shops.size(); ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(speed));
        std::unique_lock<std::mutex> lock(mutex);
        av_work_time += (speed * int(queue_B.size())
            / double(count));
        time += speed * int(queue_B.size());
        av_wait_time += (speed * (count - int(queue_B.size()))
            / double(count));
        lock.unlock();
    }
}

void Shop::Serv_Q(queueCust queue) {
    while (!queue->empty()) {
        Customer curr = *queue->front();
        queue->pop();
        Serv_customers(curr);
        av_time = (av_time * served + speed * curr.shops.size())
            / double(served + 1.0);
        served++;
    }
}

int Shop::Served() {
    return served;
}

int Shop::Unserved() {
    return max_customer - served;
}

double Shop::Av_len() {
    return av_lenght;
}

double Shop::Av_time() {
    return av_time * 0.001;
}

double Shop::Av_work_time() {
    return av_work_time * 0.001;
}

double Shop::Av_wait_time() {
    return av_wait_time * 0.001;
}

double Shop::Failure_probability() {
    double ans = static_cast<double>(Unserved()) / this->max_customer;
    return ans;
}

double Shop::Relative_throughput() {
    double ans = static_cast<double>(Served()) / max_customer;
    return ans;
}

double Shop::Absolute_throughput() {
    double ans = Served() / (time * 0.001);
    return ans;
}

void Shop::Statistic() {
    std::cout << "Servised: " <<
        Served() << std::endl;
    std::cout << "Not servised: " <<
        Unserved() << std::endl;
    std::cout << "Average queue length: " <<
        Av_len() << std::endl;
    std::cout << "Average customer waiting time: " <<
        Av_time() << std::endl;
    std::cout << "Average checkout work time: " <<
        Av_work_time() << std::endl;
    std::cout << "Average checkout wait time: " <<
        Av_wait_time() << std::endl;
    std::cout << "Probability of failure: " << Failure_probability() << std::endl;
    std::cout << "Relative throughput: " << Relative_throughput() << std::endl;
    std::cout << "Absolute throughput: " << Absolute_throughput() << std::endl;
}
