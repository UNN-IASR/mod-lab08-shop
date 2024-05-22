// Copyright 2024 MISha
#include "../include/task.h"
#include <iostream>
#include <thread>
#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <chrono>
#include <mutex>
#include <map>
int F(int n) {
    if (n == 0)
        return 1;
    else
    {
        return n * F(n - 1);
    }
       
}
int RandomTime(int customerIntensity) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<int> dis(customerIntensity - 1);
    return static_cast<int>(dis(gen));
}

int RandomProduct(int avgProductCount) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> dis(avgProductCount);
    int count = dis(gen);
    if (count == 0) return 1;
    else return count+10;
}
int main()
{   
    int QueueLength = 5;
    int ClientCount = 1000;
    int CassaCount = 3;
    double Intensity = 40;
    int Speed = 50;
    int CountProduct = 3;
    double mu = 1000.0 / Speed / CountProduct;
    double ro = Intensity / mu;

    double P0 = 0;
    for (int i = 0; i <= CassaCount; i++)
    {
        P0 += pow(ro, i) / F(i);
    }
    for (int i = 1; i <= QueueLength; i++)
    {
        P0 += pow(ro, CassaCount + i) / (pow(CassaCount, i) * F(CassaCount));
    }
    P0 = 1 / P0;
    double Pq = P0 * (pow(ro, CassaCount) / F(CassaCount)) * ((1 - pow(ro / CassaCount, QueueLength)) / (1 - ro / CassaCount));
    double Prej = P0 * pow(ro, CassaCount + QueueLength) / (pow(CassaCount, QueueLength) * F(CassaCount));
    double Q = 1 - Prej;
    double A = Intensity * Q;

    std::cout << "\nQueue prob: " << Pq;
    std::cout << "\nReject prob: " << Prej;
    std::cout << "\nRel throughput: " << Q;
    std::cout << "\nSystem idle prob: " << P0;
    std::cout << "\nAbsolutli throughput: " << A;
    std::cout << "\nSystem load: " << ro;
    std::cout << std::endl << std::endl;;
	setlocale(LC_ALL, "rus");
	Shop cahs(CassaCount, QueueLength, Speed);
	srand(static_cast<unsigned int>(time(0)));
	int randomNumber;
	std::chrono::system_clock::time_point start= std::chrono::system_clock::now();
	for (int i = 0; i < ClientCount; i++)
	{
		randomNumber = (rand() % 10) + 1;
		cahs.AddNewClient(RandomProduct(CountProduct), i);
		std::this_thread::sleep_for(std::chrono::milliseconds(RandomTime(Intensity)));
	}
	do{
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();

	} while (cahs.clientQueue.size() > 0);
    std::cout << "\nReject prob: " << (double)cahs.NotBuy / ClientCount;
    std::cout << "\nRelitit throughput: " << (double)cahs.Buy / ClientCount;
    std::cout << "\nAbsalutli throughput: " << (double)cahs.Buy / ClientCount * Intensity;
}
