#include "task.h"

#include <iostream>
#include <iomanip>

int main()
{
    setlocale(LC_ALL, "Russian");
    int checkout_count = 3; // количество касс
    int product_processing_time = 50; //время обработки товара на кассе
    int max_line_length = 2; // максимальная длина очереди
    double intensity_customer = 20; // интенсивность потока клиентов
    double avarage_count_of_products = 6;//среднее количество товаров в тележке
    Model model(checkout_count, intensity_customer, product_processing_time,
        avarage_count_of_products, max_line_length);
    int time_of_work = 15 * 1000;
            

    std::cout << "Входные параметры:" << std::endl;
    std::cout << "\tКоличество касс: " << checkout_count << std::endl;
    std::cout << "\tВремя обработки товаров на кассе: "
        << product_processing_time << std::endl;
    std::cout << "\tМаксимальная длина очереди: " << max_line_length
        << std::endl;
    std::cout << "\tИнтенсивность потока клиентов: " << intensity_customer
        << std::endl;
    std::cout << "\tСреднее количество товаров в тележке покупателя: "
        << avarage_count_of_products << std::endl;

    model.Run(time_of_work);


    //Вывод всей информации



    std::cout << "\nПо результам имитации работы магазина была получена"<<
        " такая статистика:" << std::endl;
    std::cout << "\tВремя работы магазина: " << model.get_work_time()
        << std::endl;
    std::cout << "\tКоличество покупателей: " << 
        model.get_served_customers_count() + 
        model.get_unserved_customers_count() << std::endl;
    std::cout << "\tКоличество обслуженных покупателей: "
        << model.get_served_customers_count() << std::endl;
    std::cout << "\tКоличество необслуженных покупателей: "
        << model.get_unserved_customers_count() << std::endl;
    std::cout << "\tСреднее время нахождения покупателя в очереди + на кассе: "
        << model.get_avarage_customer_time() << std::endl;
    std::cout << "\tСреднее время работы кассы: " <<
        model.get_avarage_checkout_work_time() << std::endl;
    std::cout << "\tСреднее время простоя кассы: " <<
        model.get_avarage_chechout_downtime() << std::endl;

    std::cout << "\n|                                      | Теоретические"<<
        " результаты | Практические результаты  |    Абсолютная разница    |"
        << std::endl;
    std::cout << "|--------------------------------------|-------------------"
        <<"-------|--------------------------|--------------------------|"
        << std::endl;
    std::cout.setf(std::ios::fixed);
    std::cout << "| Вероятность отказа системы           | "
        << std::setw(24) << std::setprecision(13)
        << model.get_theoretical_probability_of_rejection() << " | "
        << std::setw(24) << std::setprecision(13)
        << model.get_actual_probability_of_rejection() << " | "
        << std::setw(24) << std::setprecision(13)
        << abs(model.get_theoretical_probability_of_rejection()
            - model.get_actual_probability_of_rejection()) << " |"
        << std::endl;
    std::cout << "| Относительная пропускная способность | " 
        << std::setw(24) << std::setprecision(13)
        << model.get_theoretical_relative_throughput() << " | "
        << std::setw(24) << std::setprecision(13)
        << model.get_actual_relative_throughput() << " | "
        << std::setw(24) << std::setprecision(13) 
        << abs(model.get_theoretical_relative_throughput() 
            - model.get_actual_relative_throughput()) << " |" << std::endl;
    std::cout << "| Абсолютная пропускная способность    | "
        << std::setw(24) << std::setprecision(13)
        << model.get_theoretical_absolute_throughput() << " | "
        << std::setw(24) << std::setprecision(13)
        << model.get_actual_absolute_throughput() << " | "
        << std::setw(24) << std::setprecision(13)
        << abs(model.get_theoretical_absolute_throughput()
            - model.get_actual_absolute_throughput()) << " |" << std::endl;

}
