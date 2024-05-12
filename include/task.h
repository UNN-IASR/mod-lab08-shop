#include <thread>
#include <deque>
#include <chrono>
#include <mutex>
#include <ctime>
#include <cmath>


       

struct Checkout {
    bool in_use;
    std::thread* thrd;
    long sum_work_time;
};

struct Customer {
    int id;
    int product_count;
    std::chrono::system_clock::time_point start_customer;
};

class Shop {
 private:
    int checkout_count; // количество касс
    int product_processing_time; //время обработки товара на кассе
    int max_line_length; // максимальная длина очереди

    int served_customers_count=0; //количество обслуженных клиентов
    int unserved_customers_count=0; // количество необслуженных клиентов
    long sum_line_length=0; // сумма произведений длины линии на время для нахождения средней длины
    long sum_customer_time=0; // суммарное время нахождения покупателя в очереди + на кассе
    long work_time=0; // время работы магазина

    std::chrono::system_clock::time_point start_work;
    std::chrono::system_clock::time_point start_line_length;

    Checkout* checkouts; // массив касс
    std::deque<Customer> line; // вектор клиентов в очереди

    void serve(Customer customer, int checkout_number);

    std::mutex mu;
    std::mutex mu1;

 public:
    Shop();
    Shop(int _checkout_count,
        int _product_processing_time,
        int _max_line_length);
    Shop(const Shop &s);

    void Open();
    void Handle_customer(Customer customer);
    void Close();


    int get_served_customers_count(); //количество обслуженных клиентов
    int get_unserved_customers_count(); // количество необслуженных клиентов
    double get_avarage_line_length(); // средняя длина очереди
    double get_avarage_customer_time(); // среднее время нахождения покупателя
                                        // в очереди + на кассе
    double get_avarage_checkout_work_time(); // среднее время работы кассы
    double get_avarage_chechout_downtime(); // среднее время простоя кассы
    long get_work_time();
};


class Model {
 private:

    int checkout_count; // количество касс
    int product_processing_time; // время обработки товара на кассе
    int max_line_length; // максимальная длина очереди
    double intensity_customer; // интенсивность потока клиентов
    double avarage_count_of_products; // среднее количество товаров в тележке

    Shop shop;

    double theoretical_probability_of_rejection; //теоретическая веротность отказа
    double theoretical_relative_throughput; //теоретическая относительная пропускная способность
    double theoretical_absolute_throughput; //теоретическая абсолютная пропускная способность

    double actual_probability_of_rejection; // практическая веротность отказа
    double actual_relative_throughput; // практическая относительная пропускная способность
    double actual_absolute_throughput; // практическая абсолютная пропускная способность


    double actual_avarage_count_of_products;
    double actual_intensity_of_customers;
 public:
    Model(int _checkout_count, double _intensity_customer,
        int _product_processing_time, double _avarage_count_of_products,
        int _max_line_length);

    void Run(int time_of_work);
    int get_random_number(double avarage, double standart_deviation);

    double get_theoretical_probability_of_rejection();//теоретическая веротность отказа
    double get_theoretical_relative_throughput();//теоретическая относительная пропускная способность
    double get_theoretical_absolute_throughput();//теоретическая абсолбтная пропускная способность

    double get_actual_probability_of_rejection();//практическая веротность отказа
    double get_actual_relative_throughput(); //практическая относительная пропускная способность
    double get_actual_absolute_throughput(); //практическая абcолютная пропускная способность

    int get_served_customers_count(); //количество обслуженных клиентов
    int get_unserved_customers_count(); // количество необслуженных клиентов
    double get_avarage_line_length(); // средняя длина очереди
    double get_avarage_customer_time(); // среднее время нахождения покупателя в очереди + на кассе
    double get_avarage_checkout_work_time(); // среднее время работы кассы
    double get_avarage_chechout_downtime(); // среднее время простоя кассы
    long get_work_time();

    double get_actual_avarage_count_of_products();
    double get_actual_intensity_of_customers();
};
