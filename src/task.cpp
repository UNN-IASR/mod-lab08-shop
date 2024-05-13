#include "task.h"
// CLient

Client::Client(int count_product) {
    this->count_product = count_product;
    this->isServiced = false;
}

Client::Client(const Client& client)
{
    this->count_product = client.count_product;
    this->isServiced = client.isServiced;
    this->start_time = client.start_time;
    this->end_time = client.end_time;
}


void Client::StartWait() {
    this->start_time = std::chrono::system_clock::now();
}

void Client::EndWait() {
    this->end_time = std::chrono::system_clock::now();
}

std::chrono::milliseconds Client::GetServiceTime() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
}

int Client::GetCountProduct() {
    return this->count_product;
}

// Register

Register::Register() {
    this->countClient = 0;
    this->work_time = std::chrono::milliseconds(0);
}

Register::Register(const Register& registers)
{
    this->countClient = registers.countClient;
    this->work_time = registers.work_time;
}

void Register::RunService(Client client, int speed_service) {
    std::chrono::system_clock::time_point time_start = std::chrono::system_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(speed_service * client.GetCountProduct()));
    this->countClient++;

    std::chrono::system_clock::time_point time_end = std::chrono::system_clock::now();

    this->work_time += std::chrono::duration_cast<std::chrono::milliseconds>(time_end - time_start);
}

int Register::GetCountClient() {
    return this->countClient;
}

std::chrono::milliseconds Register::GetWorkTime() {
    return this->work_time;
}

//Statistics

Statistics::Statistics(int count_services, int count_failures, int count_visitors,
    int avg_len_client_queue, std::chrono::milliseconds avg_time_serviced,
    double intensity_client, int speed_service, int count_registers, int maxLen_queue,
    std::chrono::milliseconds time_work, std::chrono::milliseconds avg_serviced_time_work,
    std::chrono::milliseconds avg_serviced_downtime)
    : count_serviced(count_services),
    count_failures(count_failures),
    count_visitors(count_visitors),
    avg_len_client_queue(avg_len_client_queue),
    avg_time_serviced(avg_time_serviced),
    lambda(intensity_client),
    mu(1000 / speed_service),
    count_registers(count_registers),
    maxLen_queue(maxLen_queue),
    time_work(time_work),
    avg_serviced_time_work(avg_serviced_time_work),
    avg_serviced_downtime(avg_serviced_downtime)
{
    n = count_registers;
    m = maxLen_queue;
}

int Statistics::Factorial(int n) {
    int ans = 1;

    if (n == 0)
        return ans;

    for (int i = 1; i <= n; i++)
        ans *= i;
    return ans;
}

double Statistics::P0_theor() {
    double res = 0;
    double factorial_n = Factorial(n);
    double ro = this->lambda / this->mu;

    for (int i = 0; i <= n; i++)
        res += pow(ro, i) / Factorial(i);

    for (int i = 1; i <= m; i++)
        res += pow(ro, n + i) / (pow(n, i) * factorial_n);

    return 1 / res;
}

double Statistics::Prej_theor() {
    double ro = this->lambda / this->mu;

    return (pow(ro, n + m) * P0_theor()) / (pow(n, m) * Factorial(n));
}

double Statistics::Q_theor() {
    return 1.0 - Prej_theor();
}

double Statistics::A_theor() {
    return lambda * Q_theor();
}

double Statistics::Prej_real() {
    return (double)count_failures / count_visitors;
}

double Statistics::Q_real() {
    return 1.0 - Prej_real();
}

double Statistics::A_real() {
    return (double)(count_serviced) / time_work.count() * 1000;
}

std::string Statistics::ToString()
{
    std::string res = "Statistics: ";
    //res += "";
    //res += "\nNumber of ticket offices(service channels): " + count_registers;
    //res += "\nThe intensity of the flow of customers(service requests): " + std::to_string(lambda);
    //res += "\nThe processing speed of the goods at the checkout" + std::to_string(1000 / mu);
    //res += "\nThe average number of goods in the customer's cart" + 
    //res += "\nMaximum queue length"

    res += "\n\nServiced customers: " + std::to_string(count_serviced);
    res += "\nUnserved customers: " + std::to_string(count_failures);
    res += "\nAll customers: " + std::to_string(count_visitors);
    res += "\nAverage queue length: " + std::to_string(avg_len_client_queue);
    res += "\nAverage service time: " + std::to_string(avg_time_serviced.count());
    res += "\nAverage opening time of the cash register: " + std::to_string(avg_serviced_time_work.count());
    res += "\nAverage downtime of the cash register: " + std::to_string(avg_serviced_downtime.count());
    res += "\nAll time work: " + std::to_string(time_work.count());

    res += "\n\nTheoretical probability of failure: " + std::to_string(Prej_theor());
    res += "\nTheoretical relative throughput of the store: " + std::to_string(Q_theor());
    res += "\nTheoretical absolute throughput: " + std::to_string(A_theor());

    res += "\n\nReal probability of failure: " + std::to_string(Prej_real());
    res += "\nReal relative throughput of the store: " + std::to_string(Q_real());
    res += "\nReal absolute throughput: " + std::to_string(A_real());

    return res;
}

// Shop

Shop::Shop(int count_registers, double intensity, int speed_service, int count_product, int maxLen_queue) {
    this->count_registers = count_registers;
    this->client_intensity = intensity;
    this->speed_service = speed_service;
    this->count_product = count_product;
    this->maxLen_client_queue = maxLen_queue;

    this->count_failures = 0;
    this->count_serviced = 0;
    this->count_visitors = 0;
    this->avg_len_client_queue = 0;
    this->avg_time_serviced_client = std::chrono::milliseconds(0);
    this->time_work = std::chrono::milliseconds(0);

    this->isEnd = false;
    this->count_tact = 0;
}

void Shop::Run(std::chrono::milliseconds time_run) {
    for (int i = 0; i < count_registers; i++)
        this->register_queue.push(Register());
    auto start_t = std::chrono::system_clock::now();
    auto end_t = std::chrono::system_clock::now();

    std::thread thr_add_client(&Shop::AddClient, this);
    std::thread thr_listen(&Shop::Listen, this);

    std::chrono::milliseconds time = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);
    while (time < time_run) {
        end_t = std::chrono::system_clock::now();
        time = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);
    }

    mtx_for_time.lock();
    this->isEnd = true;
    mtx_for_time.unlock();

    thr_add_client.join();
    thr_listen.join();

    end_t = std::chrono::system_clock::now();
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end_t - start_t);

    this->avg_len_client_queue /= this->count_tact;
    this->avg_time_serviced_client /= this->count_serviced;

    while (register_queue.size() > 0) {
        this->avg_register_time_work += register_queue.front().GetWorkTime();
        register_queue.pop();
    }
    this->avg_register_time_work /= count_registers;
    this->avg_register_downtime = time - this->avg_register_time_work;
    this->time_work = time;
}

Client Shop::SpawnClient() {
    int count_product = (rand() % this->count_product) + 1;
    return Client(count_product);
}

void Shop::AddClient() {
    bool flag = false;
    do {
        mtx_for_time.lock();
        flag = this->isEnd;
        mtx_for_time.unlock();

        Client client = SpawnClient();
        this->count_visitors++;

        mtx_for_client_queue.lock();
        int size_client_queue = client_queue.size();
        mtx_for_client_queue.unlock();

        if (size_client_queue >= this->maxLen_client_queue)
            this->count_failures++;
        else {
            mtx_for_client_queue.lock();
            client.StartWait();
            client_queue.push(client);
            mtx_for_client_queue.unlock();
        }

        this->count_tact++;
        this->avg_len_client_queue += client_queue.size();
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1 / this->client_intensity * 1000)));
    } while (!flag);
}

void Shop::RunRegister(Client client, Register registers) {
    registers.RunService(client, speed_service);
    client.EndWait();
    this->count_serviced++;

    mtx_for_register_queue.lock();
    register_queue.push(registers);
    mtx_for_register_queue.unlock();

    this->avg_time_serviced_client += client.GetServiceTime();
}

void Shop::Listen() {
    bool flag = false;
    do {
        mtx_for_time.lock();
        flag = this->isEnd;
        mtx_for_time.unlock();

        mtx_for_client_queue.lock();
        mtx_for_register_queue.lock();
        int client_queue_size = client_queue.size();
        int register_queue_size = register_queue.size();
        mtx_for_client_queue.unlock();
        mtx_for_register_queue.unlock();

        if (client_queue_size == 0 || register_queue_size == 0) {
            std::this_thread::yield;
            continue;
        }

        mtx_for_client_queue.lock();
        Client client = client_queue.front();
        client_queue.pop();
        mtx_for_client_queue.unlock();

        mtx_for_register_queue.lock();
        Register registers = register_queue.front();
        register_queue.pop();
        mtx_for_register_queue.unlock();

        std::thread thr(&Shop::RunRegister, this, client, registers);
        thr.detach();
    } while (!flag || !client_queue.empty() || register_queue.size() < this->count_registers);
}

Statistics Shop::GetStatistics() {
    return Statistics(
        count_serviced,
        count_failures,
        count_visitors,
        avg_len_client_queue,
        avg_time_serviced_client,
        client_intensity,
        speed_service,
        count_registers,
        maxLen_client_queue,
        time_work,
        avg_register_time_work,
        avg_register_downtime);
}