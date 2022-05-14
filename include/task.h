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
	void inQueue();
	void outQueue();
};
class Shop {
public:
	int countCashbox;
	int maxQueue;
	int serviceSpeed;
	int requestRate;
	int avgItems;
	
	int completedClients;
	int rejectedClients;
	
	struct Cashbox {
		int tWait;
		int tWork;
		std::thread thrd;
	};

	std::vector<Cashbox> cashboxes;
	std::deque<Client*> queueClients;
	std::mutex m;

	Shop(int countCashbox, int requestRate, int serviceSpeed, int avgItems, int maxCount);
	void Start(int index);
	void Simulation();
};
