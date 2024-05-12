//Copyright Annenko Misha 2024
#include "task.h"


void Help_main() {
    const int count_mall = 5;
    const int lambda = 100;
    const int Service_time = 100;
    const int average_products = 4;
    const int large_queue_length = 5;

    int pass_shoppers = 0;
    int miss_shoppers = 0;
    double average_queue_length = 0;
    double average_overall_time = 0;
    double average_mall_time = 0;
    double average_wait_time = 0;

    double Prej = 0;
    double Q = 0;
    double A = 0;

    Shopping_mall array[count_mall];
    for (int i = 0; i < count_mall; i++) {
        array[i] = Shopping_mall(Service_time);
        array[i].generation();
    }

    Shopper shoppers0[100];
    std::vector<double> queues, times_at_mall;
    for (int j = 0; j < 100; j++) {
        shoppers0[j] = Shopper(average_products, j);
        int _num_ = Cash_Choose(array, count_mall, large_queue_length);
        if (_num_ != -1) {
            array[_num_].Push_back_shopper(shoppers0[j]);
            std::cout << "Клиент " << j + 1 <<
                " встал в очередь кассы " << _num_ + 1 << std::endl;
            pass_shoppers++; //
        }
        else {
            miss_shoppers++; //
            std::cout << "Клиент " << j + 1 <<
                " не смог встать в очередь и ушел " << std::endl;
        }

        std::vector<double> avg_iter;
        for (int i = 0; i < count_mall; i++) {
            avg_iter.push_back(array[i].shoppers.size());
        }
        queues.push_back(Calculate_Average(avg_iter));

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / lambda));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    for (int i = 0; i < count_mall; i++) {
        array[i].End();
        times_at_mall.push_back(Calculate_Average(array[i].duration_work_shopper()));
    }

    std::vector<double> work_time, wait_time;
    for (int i = 0; i < count_mall; i++) {  //avg of *cash_count* casses
        work_time.push_back(Calculate_Average(array[i].working_time));
        wait_time.push_back(Calculate_Average(array[i].Return_waiting_time()));
    }

    average_queue_length = Calculate_Average(queues);
    average_overall_time = Calculate_Average(times_at_mall);
    average_mall_time = Calculate_Average(work_time);
    average_wait_time = Calculate_Average(wait_time);

    Prej = (double)miss_shoppers / (miss_shoppers + pass_shoppers);
    Q = 1 - Prej;
    A = Q * lambda;

    std::cout << "\n\nЭкспериментально вычисленные данные: " << std::endl;
    std::cout << "  Число обслуженных клиентов: " <<
        pass_shoppers << std::endl;
    std::cout << "  Число необслуженных клиентов: " <<
        miss_shoppers << std::endl;
    std::cout << "  Средняя длина очереди: " << average_queue_length
        << std::endl;
    std::cout << "  Среднее время в очереди + на кассе: " <<
        average_overall_time << std::endl;
    std::cout << "  Среднее время работы кассы: " <<
        average_mall_time << std::endl;
    std::cout << "  Среднее время простоя кассы: " <<
        average_wait_time << std::endl;
    std::cout << "  Вероятность отказа: " << Prej << std::endl;
    std::cout << "  Относительная пропускная способность: "
        << Q << std::endl;
    std::cout << "  Абсолютная пропускная способность: "
        << A << std::endl;
    std::cout << std::endl;

    double myu = 1000 / Service_time;
    double ro = lambda / myu;
    double sum = 0;
    for (int i = 0; i <= count_mall + large_queue_length; i++) {
        if (i <= count_mall) {
            sum += Degree(ro, i) / Factorial_Recursive(i);
        }
        else {
            sum += Degree(ro, i) / (Factorial_Recursive(count_mall) *
                Degree(count_mall, i - count_mall));
        }
    }
    double P0 = (double)1 / sum;
    Prej = P0 * Degree(ro, count_mall + large_queue_length) /
        (Degree(count_mall, large_queue_length) * Factorial_Recursive(count_mall));
    Q = 1 - Prej;
    A = lambda * Q;

    std::cout << "Теоретически вычисленные данные: " << std::endl;
    std::cout << "  Интенсивность потока клиентов: " << lambda << std::endl;
    std::cout << "  Интенсивность обслуживания: " << myu << std::endl;
    std::cout << "  Интенсивность нагрузки канала: " << ro << std::endl;
    std::cout << "  Вероятность отказа: " << Prej << std::endl;
    std::cout << "  Относительная пропускная способность: " << Q
        << std::endl;
    std::cout << "  Абсолютная пропускная способность: " << A << std::endl;
}


int main()
{
    setlocale(LC_ALL, "Rus");
    Help_main();

    //В VS не работает google test
    //ТЕСТ1
    Shopper shopper = Shopper(5, 1);
    int count = shopper.amount_of_products;
    if (count == 5) {
        std::cout << "\n\nЧётко 1";
    }
    //ТЕСТ2
    Shopping_mall mall = Shopping_mall(20);
    bool check = mall.in_service;
    if (check == false) {
        std::cout << "\nЧётко 2";
    }
    //ТЕСТ3
    Shopping_mall mall1 = Shopping_mall(20);
    Shopper shopper1 = Shopper(4, 2);
    Shopper shopper2 = Shopper(3, 1);
    mall1.Push_back_shopper(shopper1);
    mall1.Push_back_shopper(shopper2);
    int count1 = mall1.shoppers.size();
    if (count1 == 2) {
        std::cout << "\nЧётко 3";
    }
    //ТЕСТ4
    Shopping_mall mall2 = Shopping_mall(20);
    int number = Cash_Choose(&mall2, 2, 4);
    if (number == 0) {
        std::cout << "\nЧётко 4";
    }
    //ТЕСТ5
    Shopping_mall mall3 = Shopping_mall(20);
    double k = 3;
    while (k < 12) {
        mall3.Push_back_working_time((double)k);
        k++;
    }
    std::vector<double> times = mall3.working_time;
    if (times[4] == (double)7) {
        std::cout << "\nЧётко 5";
    }
}