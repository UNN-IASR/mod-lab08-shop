#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>

using namespace std;

class Customer
{
public:
	std::chrono::system_clock::time_point start;
	Customer();
	int wait();
};

class Shop
{
public:
	int count;
	int cust_count;
	int Time;
	int buy_count;
	int length;
	int rejectedCount = 0;
	int processedCount = 0;
	int requestCount = 0;
	double buy_time = 0;
	double wait_time = 0;
	double downtime = 0;
	double work_time = 0;
	double Prej = 0;
	double Q = 0;
	double A = 0;
	vector<thread> threads;
	int lenQueue = 0;
	vector<Customer> customers;
	mutex mut;
	Shop(int count, int cust_count, int Time, int buy_count, int length);
	void Work();
	//void Work(bool without_debug);
	void Start_working();
	void Start_working(bool without_debug);
};
