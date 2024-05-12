#include "task.h"
 
Shop::Shop() :
    checkout_count(0),
    product_processing_time(0),
    max_line_length(0) {
    checkouts = new Checkout[0];
}

Shop::Shop(int _checkout_count,
    int _product_processing_time,
    int _max_line_length) :
    checkout_count(_checkout_count),
    product_processing_time(_product_processing_time),
    max_line_length(_max_line_length) {
    checkouts = new Checkout[_checkout_count];
}

void Shop::Open() {
    
    served_customers_count = 0;
    unserved_customers_count = 0; 
    sum_line_length = 0; 
    sum_customer_time = 0; 
    for (int i = 0; i < checkout_count;i++) {
        checkouts[i].in_use = false;
        checkouts[i].sum_work_time = 0;
    }
    line.clear();

    start_work = std::chrono::system_clock::now();
    start_line_length = std::chrono::system_clock::now();
    
}

void Shop::Handle_customer(Customer customer) {
    mu.lock();
    customer.start_customer = std::chrono::system_clock::now();
    for (int i = 0; i < checkout_count; i++){
        if (!checkouts[i].in_use) {
            checkouts[i].in_use = true;
            checkouts[i].thrd = new std::thread(&Shop::serve,
                this, customer, i);
            mu.unlock();
            return;
        }
    }
    if ((int)line.size() < max_line_length) {
        
        std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
        sum_line_length+=line.size()* (long)std::chrono::duration_cast<std::chrono::milliseconds>(end-start_line_length).count();
        line.push_back(customer);
        start_line_length = end;
    }
    else {
        unserved_customers_count++;
    }
    mu.unlock();
}

void Shop::serve(Customer customer, int checkout_number){
    std::this_thread::sleep_for(std::chrono::milliseconds(product_processing_time * customer.product_count));
    mu1.lock();
    checkouts[checkout_number].sum_work_time += product_processing_time * customer.product_count;
    served_customers_count++;
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    sum_customer_time+= (long)std::chrono::duration_cast<std::chrono::milliseconds>(end- customer.start_customer).count();
    if (line.size() > 0) {
        Customer c = line.front();
        sum_line_length += line.size() * (long)std::chrono::duration_cast<std::chrono::milliseconds>(end- start_line_length).count();
        line.pop_front();
        start_line_length = end;
        mu1.unlock();
        serve(c, checkout_number);
    }
    else {
        mu1.unlock();
        checkouts[checkout_number].in_use = false;
    }
}

void Shop::Close() {
    for (int i = 0; i < checkout_count; i++) {
        checkouts[i].thrd->join();
    }
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    work_time = (long)std::chrono::duration_cast<std::chrono::milliseconds>(end-start_work).count();
}

int Shop::get_served_customers_count(){
    return served_customers_count;
}
int Shop::get_unserved_customers_count() {
    return unserved_customers_count;
}

double Shop::get_avarage_line_length() {
    return (double)sum_line_length / work_time;
}

double Shop::get_avarage_customer_time() {
    return (double)sum_customer_time / (served_customers_count + unserved_customers_count);
}

double Shop::get_avarage_checkout_work_time() {
    long sum = 0;
    for (int i = 0; i < checkout_count; i++) {
        sum += checkouts[i].sum_work_time;
    }
    return (double)sum / checkout_count;
}

double Shop::get_avarage_chechout_downtime() {
    return work_time - get_avarage_checkout_work_time();
}

long Shop::get_work_time() {
    return work_time;
}

Model::Model(int _checkout_count, double _intensity_customer,
    int _product_processing_time, double _avarage_count_of_products,
    int _max_line_length) :
    checkout_count(_checkout_count),
    intensity_customer(_intensity_customer),
    product_processing_time(_product_processing_time),
    avarage_count_of_products(_avarage_count_of_products),
    max_line_length(_max_line_length),
    shop(_checkout_count, _product_processing_time, _max_line_length) {

    double load_intensity = intensity_customer / (1000 / (product_processing_time * avarage_count_of_products));
    
    //Поиск теоретической вероятности отказа системы, 
    //относительной и абсолютной пропускной способности
    double sum=1;
    double pow_p=1;
    long factorial = 1;
    for (int i = 1; i <= checkout_count;i++) {
        pow_p *= load_intensity;
        factorial *= i;
        sum += pow_p / factorial;
    }
    long pow_n = 1;
    for (int i = 1; i <= max_line_length; i++) {
        pow_p *= load_intensity;
        pow_n *= checkout_count;
        sum += pow_p / (pow_n * factorial);
    }
    double probability_all_channels_are_free = pow(sum,-1);
    theoretical_probability_of_rejection = probability_all_channels_are_free * pow_p / (pow_n * factorial);
    theoretical_relative_throughput = 1 - theoretical_probability_of_rejection;
    theoretical_absolute_throughput = theoretical_relative_throughput * intensity_customer;
    actual_probability_of_rejection = 0;
    actual_relative_throughput = 0;
    actual_absolute_throughput = 0;

} 


int Model::get_random_number(double avarage, double standart_deviation) {
    double pi = 3.14159265358979323846;
    double a = sqrt(2 * pi) * standart_deviation;
    int max_f = (int)round(RAND_MAX * (1 / a));
    double f = (double) (rand()%max_f) / RAND_MAX;
    double x = sqrt(-2*log(f * a))*standart_deviation;
    double max_x = sqrt(-2 * log(0.0001 * a)) * standart_deviation;
    int rand_num = -1 + 2 * (rand() % 2);
    int result = (int)round(avarage + rand_num * x * avarage / max_x);
    return result;
}

void Model::Run(int time_of_work) {
    //Для рандомизации времени прихода клиентов и количества их продуктов 
    //используем нормальное распределение случайной величины

    double avarage_time_between_clients = 1000 / intensity_customer;
    double sd_time_between_clients = 0.7; //среднее квадратическое для
                                            // времени между клиентами
    double sd_product_count = 0.7;//среднее квадратичное количества продуктов
    srand((int)time(0));
    int time_between_customers = get_random_number(avarage_time_between_clients, sd_time_between_clients);
    long sum_time=time_between_customers;
    int id= 0;
    int sum_of_products = 0;
    shop.Open(); // Открываем магазин
    while (sum_time <= time_of_work) {
        id++;
        Customer c;
        c.id = id;
        c.product_count = get_random_number(avarage_count_of_products, sd_product_count);
        sum_of_products += c.product_count;
        shop.Handle_customer(c);
        std::this_thread::sleep_for(std::chrono::milliseconds(time_between_customers));
        time_between_customers = get_random_number(avarage_time_between_clients, sd_time_between_clients);
        sum_time += time_between_customers;
    }
    shop.Close();

    int customers_count = shop.get_served_customers_count() + shop.get_unserved_customers_count();
    actual_probability_of_rejection = (double)shop.get_unserved_customers_count() / customers_count;
    actual_relative_throughput = (double)shop.get_served_customers_count()/customers_count;
    actual_absolute_throughput = shop.get_served_customers_count() / ((double)shop.get_work_time()/1000);
    actual_avarage_count_of_products = (double)sum_of_products / customers_count;
    actual_intensity_of_customers = 1000/((double)sum_time / customers_count);

}


double Model::get_theoretical_probability_of_rejection() {
    return theoretical_probability_of_rejection;
}

double Model::get_theoretical_relative_throughput() {
    return theoretical_relative_throughput;
}

double Model::get_theoretical_absolute_throughput() {
    return theoretical_absolute_throughput;
}

double Model::get_actual_probability_of_rejection() {
    return actual_probability_of_rejection;
}

double Model::get_actual_relative_throughput() {
    return actual_relative_throughput;
}

double Model::get_actual_absolute_throughput() {
    return actual_absolute_throughput;
}

int Model::get_served_customers_count() {
    return shop.get_served_customers_count();
}
int Model::get_unserved_customers_count() {
    return shop.get_unserved_customers_count();
}
double Model::get_avarage_line_length() {
    return shop.get_avarage_line_length();
}
double Model::get_avarage_customer_time() {
    return shop.get_avarage_customer_time();
}
double Model::get_avarage_checkout_work_time() {
    return shop.get_avarage_checkout_work_time();
}
double Model::get_avarage_chechout_downtime() {
    return shop.get_avarage_chechout_downtime();
}
long Model::get_work_time() {
    return shop.get_work_time();
}

double Model::get_actual_avarage_count_of_products() {
    return actual_avarage_count_of_products;
}

double Model::get_actual_intensity_of_customers() {
    return actual_intensity_of_customers;
}
