#include "task.h"
#include <iostream>
#include <cmath>

Shop::Shop(int n1, double n2, double n3, int n4, int n5) {
    numberCass = n1; //���������� ����
    intensityOfCustomerFlow = n2; //������������� ������ �����������
    speedProcessingCheckout = n3; //�������� ��������� ������ �� �����
    averageOfItemInCart = n4; // ������� ���������� ������� � ������� ����������
    maxQueueLength = n5; //������������ ����� �������

    queueCustomers = std::queue<Customer>();
    vectorCass = std::vector<Cassa>();
    threadCass = std::vector<std::thread*>();

    //������ ������ ����
    for (int i = 0; i < numberCass; i++) {
        Cassa cassa;
        vectorCass.push_back(cassa);
        vectorCass[i].timeStart = std::chrono::system_clock::now();
    }
}

Shop::~Shop() {
}

//���������� ������������� ������� ��������.
void Shop::Start() {
    allTimeStart = std::chrono::system_clock::now();
    for (int i = 0; i < 50; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        srand(time(NULL));
        int _random = 1 + rand() % 100;
        //��������� ����������
        if (_random <= intensityOfCustomerFlow*100) {
            Customer customer;
            customer.countItems = rand() % 7 + averageOfItemInCart + 1;
            customer.timeStart = std::chrono::system_clock::now();
            //��������� ��� � ������� ��� ��������
            if (queueCustomers.size() < maxQueueLength) {
                queueCustomers.push(customer);  
            } else {
                unservedCustomers++;
            }
            //���������, ���� �� ��������� �����
            for (int j = 0; j < numberCass; j++) {
                if (vectorCass[j].isWork == false && queueCustomers.size() != 0) {
                    vectorCass[j].isWork = true;
                    queueCustomers.front().timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::system_clock::now() - queueCustomers.front().timeStart).count();
                    threadCass.push_back(new std::thread(&Shop::Cash, this, j,
                        queueCustomers.front(), speedProcessingCheckout*100, i));

                    averageTimeInQueue += queueCustomers.front().timeInQueue;
                    averageTimeInCass += (int)(queueCustomers.front().countItems / (speedProcessingCheckout * 100));
                    servedCustomers++;
                    queueCustomers.pop();
                }
            }  
        }
        averageLenghtQueue += queueCustomers.size();
    }
    for (auto& th : threadCass) th->join();
    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();

     

}
//������������� ������ �����
void Shop::Cash(int i, Customer customer, double speed, int j) {
    std::unique_lock<std::mutex> mu(myMutex);
    vectorCass[i].timePause += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - vectorCass[i].timeStart).count();
    vectorCass[i].timeWork += (int)(customer.countItems / speed);
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(customer.countItems / speed)));
    vectorCass[i].timeStart = std::chrono::system_clock::now();
    vectorCass[i].isWork = false;
    mu.unlock();
} 

void Shop::Stata() {
    std::cout << "����� �����������: " << servedCustomers + unservedCustomers << std::endl;
    std::cout << "�� ���:" << std::endl;
    std::cout << "���������: " << servedCustomers << std::endl;
    std::cout << "�������: " << unservedCustomers << std::endl;
    averageLenghtQueue = averageLenghtQueue / 50;
    std::cout << "������� ����� �������: " << averageLenghtQueue << std::endl;
    averageTimeInQueue = averageTimeInQueue / servedCustomers;
    averageTimeInCass = averageTimeInCass / servedCustomers;
    std::cout << "������� ����� ���������� � �������: " << averageTimeInQueue << " ms" << std::endl;
    std::cout << "������� ����� ���������� �� �����: " << averageTimeInCass << " ms" << std::endl;

    for (int i = 0; i < numberCass; i++) {
        averageTimeWorkCass += vectorCass[i].timeWork;
        averageTimeSleepCass += vectorCass[i].timePause;
    }
    averageTimeWorkCass = averageTimeWorkCass / numberCass;
    averageTimeSleepCass = averageTimeSleepCass / numberCass;

    std::cout << "������� ����� ������ �����: " << averageTimeWorkCass << " ms" << std::endl;
    std::cout << "������� ����� ������� �����: " << averageTimeSleepCass << " ms" << std::endl;

    std::cout << "////////////�� ������ ����������� ������////////////" << std::endl;

    std::cout << "����������� ������: " << (double)unservedCustomers/ (double)(unservedCustomers + servedCustomers) << std::endl;
    std::cout << "������������� ���������� ����������� ��������: " << (double)servedCustomers/ (double)(unservedCustomers + servedCustomers) << std::endl;
    std::cout << "���������� ���������� ����������� ��������: " << (double)servedCustomers / (double)(unservedCustomers + servedCustomers)* (intensityOfCustomerFlow*100) << std::endl;

    std::cout << "////////////������������� ��������////////////" << std::endl;

    //������������� �������� ������ 
    double ro = (1/intensityOfCustomerFlow) / (1/speedProcessingCheckout);
    int m = maxQueueLength;
    int n = numberCass;

    std::cout << "������������� �������� ������: " << ro << std::endl;

    //�����������, ��� ��� ������ ��������
    double Po = 1;
    for (int i = 1; i <= n; i++) {
        Po += ((double)pow(ro, i) / (double)Factorial(i));
    }
    for (int i = 1; i <= m; i++) {
        Po += ((double)pow(ro, n+i) / pow(n, i)*(double)Factorial(n));
    }
    Po = pow(Po, -1);

    std::cout << "�����������, ��� ��� ������ ��������: " << Po << std::endl;

    double Prej = Po * ((double)pow(ro, n + m) / (double)(pow(n, m) * Factorial(n)));
    std::cout << "����������� ������: " << Prej << std::endl;
    
    double Q = 1 - Prej;
    std::cout << "������������� ���������� ����������� ��������: " << Q << std::endl;

    double A = (intensityOfCustomerFlow*100) * Q;
    std::cout << "���������� ���������� ����������� ��������: " << A << std::endl;

    
    std::cout << "allTime: " << allTime << std::endl;

}

int Factorial(int i) {
    if (i <= 0)
        return 1;
    else
        return i * Factorial(i - 1);
}