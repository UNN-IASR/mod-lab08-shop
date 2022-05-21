// Copyright 2022 UNN-IASR
#pragma once
#include <vector>
#include <string>
#include <deque>
#include <mutex>
#include <chrono>
#include <cmath>
#include <thread>

class Client {
public:
	int id;
	int tQueue;
	int Products;
	int tService;
	std::chrono::system_clock::time_point timestart;

	Client(int a, int i);
	void outCashbox(int t);
	void Qin();
	void Qout();
};
class Shop {
public:
	int money;
	int maxQueue;
	int speed;
	int requestRate;
	int avgItems;

	int servedClients;
	int rejectedClients;

	struct Money {
		int tWait;
		int tWork;
		std::thread thrd;
	};

	std::vector<Money> cashboxes;
	std::deque<Client*> queueClients;
	std::mutex m;

	Shop(int M, int r, int s, int a, int c);
	void Start(int ind);
	void Work();
};
