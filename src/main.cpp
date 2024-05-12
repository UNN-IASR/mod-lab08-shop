#include "../include/task.h"

int main() {
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

    std::cout<<"��������" << std::endl;
    std::cout<<"����������� ����������:"<< cassa.ServicedCustomers << std::endl;
    std::cout<<"������������� ����������:"<< cassa.UnServicedCustomers << std::endl;
    std::cout<<"������� ����� �������:"<< (double)cassa.AverageQueueLength/NumberCustomers << std::endl;
    std::cout<<"������� ����� ���������� ���������� � �������:"<< (double)cassa.TimeQueue/NumberCustomers/1000 << std::endl;
    std::cout<<"������� ����� ������ �����:"<< (double)cassa.AverageBusyCassa/NumberCustomers/1000 << std::endl;
    std::cout<<"������� ����� ������� �����:"<< (double)(cassa.TimeCassa - cassa.AverageBusyCassa/NumberCustomers)/1000 << std::endl;
    std::cout<<"����������� ������:"<< (double)cassa.UnServicedCustomers/NumberCustomers << std::endl;
    std::cout<<"������������� ���������� ����������� ��������:"<< (double)cassa.ServicedCustomers/NumberCustomers << std::endl;
    std::cout<<"���������� ���������� �����������:"<< (double)cassa.ServicedCustomers/NumberCustomers*IntensityCustomers << std::endl;
}
