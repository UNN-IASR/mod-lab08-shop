#include "../include/task.h"

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    int CountCassa = 6;
    double IntensityCustomers = 80;
    int ProcessingSpeed = 100;
    int AverageNumberOfProducts = 6;
    int MaxLengthQueue = 10;
    int NumberCustomers = 200;

    Teoretic teoretic{CountCassa, IntensityCustomers, ProcessingSpeed, AverageNumberOfProducts, MaxLengthQueue};
    teoretic.StartCalculation();

    Tsum cassa{CountCassa, IntensityCustomers, ProcessingSpeed, AverageNumberOfProducts, MaxLengthQueue};
    cassa.Execute(NumberCustomers);

    std::cout<<"Практика" << std::endl;
    std::cout<<"Обслуженные покупатели:"<< cassa.ServicedCustomers << std::endl;
    std::cout<<"Необслуженные покупатели:"<< cassa.UnServicedCustomers << std::endl;
    std::cout<<"Средняя длина очереди:"<< (double)cassa.AverageQueueLength/NumberCustomers << std::endl;
    std::cout<<"Среднее время нахождение покупателя в очереди:"<< (double)cassa.TimeQueue/NumberCustomers/1000 << std::endl;
    std::cout<<"Среднее время работы кассы:"<< (double)cassa.AverageBusyCassa/NumberCustomers/1000 << std::endl;
    std::cout<<"Среднее время простоя кассы:"<< (double)(cassa.TimeCassa - cassa.AverageBusyCassa/NumberCustomers)/1000 << std::endl;
    std::cout<<"Вероятность отказа:"<< (double)cassa.UnServicedCustomers/NumberCustomers << std::endl;
    std::cout<<"Относительная пропускная способность магазина:"<< (double)cassa.ServicedCustomers/NumberCustomers << std::endl;
    std::cout<<"Абсолютная пропускная способность:"<< (double)cassa.ServicedCustomers/NumberCustomers*IntensityCustomers << std::endl;
}
