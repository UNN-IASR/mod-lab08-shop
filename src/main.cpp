// Copyright 2024 Danil Volkov

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "../include/task.h"

int main() {
    // int numThreads = 1; // ���������� ����
    // int maxQueueLength = 1; // ������������ ����� ������� �� ������ �����
    // int numCustomers = 3; // ����� ���������� �����������
    // double intensityCustomers = 1.0; // ������������� ������ ����������� � �������
    // int averageItems = 5; // ������� ���������� ������� � ������� ����������
    // double speedProductCash = 2.0; // �������� ��������� ������ �� ����� � �������

    int numThreads = 3; // ���������� ����
    int maxQueueLength = 5; // ������������ ����� ������� �� ������ �����
    int numCustomers = 20; // ����� ���������� �����������
    double intensityCustomers = 5.0; // ������������� ������ ����������� � �������
    int averageItems = 10; // ������� ���������� ������� � ������� ����������
    double speedProductCash = 10.0; // �������� ��������� ������ �� ����� � �������

    setlocale(LC_ALL, "");

    Shop shop(numThreads, maxQueueLength, speedProductCash);
    Client client(&shop, averageItems);

    // ������� ������ ��� ������ �����
    std::vector<std::thread> serverThreads;
    for (int i = 0; i < numThreads; ++i) {
        serverThreads.push_back(std::thread(&Shop::answer, &shop));
    }

    // ���������� �����������
    for (int id = 1; id <= numCustomers; id++) {
        client.send(id);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / intensityCustomers))); // ����� ����� �������� �����������
    }

    // ���������� ������ � ���, ��� ��� ������� ����������
    shop.setAllCustomersProcessed();

    // ���������� ���������� ���� ������� ����
    for (auto& t : serverThreads) {
        t.join();
    }

    // ������� ����������
    std::cout << "����� ��������: " << shop.getRequestCount() << std::endl;
    std::cout << "���������� ��������: " << shop.getProcessedCount() << std::endl;
    std::cout << "��������� ��������: " << shop.getRejectedCount() << std::endl;
    std::cout << "������� ����� �������: " << shop.getAverageQueueLength() << std::endl;
    std::cout << "������� ����� ���������� ���������� � �������: " << shop.getAverageQueueTime() << " ���." << std::endl;
    std::cout << "������� ����� ���������� ���������� �� �����: " << shop.getAverageCashierTime() << " ���." << std::endl;
    std::cout << "������� ����� ������ �����: " << shop.getAverageCashierTime() << " ���." << std::endl;
    std::cout << "������� ����� ������� �����: " << shop.getAverageCashierTime() - shop.getAverageQueueTime() << " ���." << std::endl;

    double probablityIdle = (double)shop.getRejectedCount() / shop.getRequestCount() * 100;
    double realtiveCapacity = (double)shop.getProcessedCount() / (shop.getProcessedCount() + shop.getRejectedCount()) * 100;
    double absoluteCapacity = intensityCustomers * realtiveCapacity;
    std::cout << "����������� ������ ��������: " << probablityIdle << "%" << std::endl;
    std::cout << "������������� ���������� ����������� ��������: " << realtiveCapacity << "%" << std::endl;
    std::cout << "���������� ���������� ����������� ��������: " << absoluteCapacity << "%" << std::endl;

    std::cout << std::endl << "���������� �� ������: " << std::endl;

    double channelLoadIntensity = intensityCustomers / speedProductCash; // ������������� �������� ������
    probablityIdle = (std::pow(channelLoadIntensity, numThreads) / factorial(numThreads))
        * (1 - std::pow(channelLoadIntensity / numThreads, maxQueueLength))
        / (1 - channelLoadIntensity / numThreads) * 100; // ����������� ������
    realtiveCapacity = 100 - probablityIdle;
    absoluteCapacity = intensityCustomers * realtiveCapacity;

    std::cout << "����������� ������ ��������: " << probablityIdle << "%" << std::endl;
    std::cout << "������������� ���������� ����������� ��������: " << realtiveCapacity << "%" << std::endl;
    std::cout << "���������� ���������� ����������� ��������: " << absoluteCapacity << "%" << std::endl;

    std::ofstream outputFile("report.txt");

    if (outputFile.is_open()) {
        outputFile << "���������� �� ���������: " << std::endl;
        outputFile << "����� ��������: " << shop.getRequestCount() << std::endl;
        outputFile << "���������� ��������: " << shop.getProcessedCount() << std::endl;
        outputFile << "��������� ��������: " << shop.getRejectedCount() << std::endl;
        outputFile << "������� ����� �������: " << shop.getAverageQueueLength() << std::endl;
        outputFile << "������� ����� ���������� ���������� � �������: " << shop.getAverageQueueTime() << " ���." << std::endl;
        outputFile << "������� ����� ���������� ���������� �� �����: " << shop.getAverageCashierTime() << " ���." << std::endl;
        outputFile << "������� ����� ������ �����: " << shop.getAverageCashierTime() << " ���." << std::endl;
        outputFile << "������� ����� ������� �����: " << shop.getAverageCashierTime() - shop.getAverageQueueTime() << " ���." << std::endl;
        outputFile << "����������� ������ ��������: " << probablityIdle << "%" << std::endl;
        outputFile << "������������� ���������� ����������� ��������: " << realtiveCapacity << "%" << std::endl;
        outputFile << "���������� ���������� ����������� ��������: " << absoluteCapacity << "%" << std::endl;
        outputFile << std::endl << "���������� �� ������: " << std::endl;
        outputFile << "����������� ������ ��������: " << probablityIdle << "%" << std::endl;
        outputFile << "������������� ���������� ����������� ��������: " << realtiveCapacity << "%" << std::endl;
        outputFile << "���������� ���������� ����������� ��������: " << absoluteCapacity << "%" << std::endl;
        
        outputFile << std::endl << "�����: �������������� ������ ���������� �� �������������,"
            << " �� ��������� ������� ������ ��������� ��������� � ���������� ��� ���������� ��������."
            << " ����� �������� ����� ���� ������� ����������� ������ ��� ������������� ������ ��������."
            << std::endl;

        outputFile.close();
    }
    else {
        std::cerr << "�� ������� ������� ���� ��� ������." << std::endl;
    }

    return 0;
}
