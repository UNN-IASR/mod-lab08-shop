#pragma once
#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <deque>
#include <vector>

class Customer {
 public:
	int countItems;
    double timeInQueue = 0;
    double timeInCass = 0;
    std::chrono::system_clock::time_point timeStart;
};

class Cassa {
 public:
    bool isWork = false;
    double timeWork = 0;
    double timePause = 0;
    std::chrono::system_clock::time_point timeStart;
};

class Shop {
 public:
    //������� ��������� ��������
    int numberCass = 0; //���������� ����
    double intensityOfCustomerFlow = 0.0; //������������� ������ �����������
    double speedProcessingCheckout = 0.0; //�������� ��������� ������ �� �����
    int averageOfItemInCart = 0; // ������� ���������� ������� � ������� ����������
    int maxQueueLength = 0; //������������ ����� �������

    //����������, ������������ ������
    int countCustomers = 0;
    std::queue<Customer> queueCustomers;//������� ��������
    std::vector<Cassa> vectorCass; //����� - ������
    std::vector<std::thread*> threadCass;
    std::mutex myMutex;

    //����������
    std::chrono::system_clock::time_point allTimeStart;
    int allTime;

    int servedCustomers = 0; //����������� �������
    int unservedCustomers = 0;  //������������� �������
    double averageLenghtQueue = 0; //������� ����� �������
    double averageTimeInQueue = 0; //������� ����� ���������� � �������
    double averageTimeInCass = 0; //������� ����� ���������� �� �����
    double averageTimeWorkCass = 0; //������� ����� ������ �����
    double averageTimeSleepCass = 0; //������� ����� ������� �����

 public:
    Shop(int n1, double n2, double n3, int n4, int n5); //������ �������� � �����������
    ~Shop();
    void Start();
    void Cash(int i, Customer customer, double speed, int j);
    void Stata();
};

int Factorial(int i);
