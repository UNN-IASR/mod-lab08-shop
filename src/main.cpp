#include "../include/task.h"
#include <iostream>

int main()
{
    setlocale(LC_ALL, "Rus");

    Store store(7, 30, 0.3, 8, 6);
    store.work();
    store.stat();

    std::cout << "обслуженные покупатели: " << store.served << std::endl;
    std::cout << "необслуженные покупатели: " << store.unserved << std::endl << std::endl;

    std::cout << "средняя длина очереди: " << store.avera << std::endl;
    std::cout << "среднее время нахождение покупателя в очереди: " << store.waiting << std::endl;
    std::cout << "среднее время нахождение покупателя на кассе: " << (int)store.serving << std::endl << std::endl;

    std::cout << "среднее время работы кассы: " << store.worked_cassa << std::endl;
    std::cout << "среднее время простоя кассы: " << store.waited_cassa << std::endl << std::endl;

    std::cout << "практические данные" << std::endl;
    std::cout << "вероятность отказа: " << store.cancel << std::endl;
    std::cout << "относительную пропускную способность магазина: " << store.o_skip << std::endl;
    std::cout << "абсолютную пропускную способность: " << store.a_skip << std::endl << std::endl;

    std::cout << "теоретические данные" << std::endl;
    std::cout << "вероятность отказа: " << store.t_cancel << std::endl;
    std::cout << "относительную пропускную способность магазина: " << store.t_o_skip << std::endl;
    std::cout << "абсолютную пропускную способность: " << store.t_a_skip << std::endl << std::endl;
    return 0;
}
