// Copyright 2021 GHA Test Team
#pragma once
#include <vector>
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <chrono>
#include <cmath>

class Customer {
public:
	int timeQueue;
	int timeServed;
	std::chrono::system_clock::time_point startTime;

	Customer(int i);
	void outCashier(int t);
	void InsideQueue();
	void OutsideQueue();
};
class Shop {
public:
	int cashierCount;
	int intencity;
	int serviceTime;
	int averageProductsCount;
	int maxQueue;
	int customerCount;

	int servedCustomers;
	int unservedCustomers;

	double spareTimeCount;
	double workTimeCount;
	double avgTimeInQueue;
	double avgTimeInServing;
	double workTime;

	struct Cashier {
		std::thread thread;
		int spareTime;
		int workTime;
	};

	std::vector<Cashier> cashierList;
	std::queue<Customer*> customersQueue;
	std::mutex mutex;

	Shop(int cashierCountTemp, int intencityTemp, int serviceTimeTemp, int averageProductsCountTemp, int maxQueueTemp, int customerCountMaxTemp);
	void ServeQueue(int index);
	void InitShop();
};