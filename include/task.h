#pragma once
#include <thread>
#include <cmath>
#include <vector>
#include <chrono>
#include <list>

class Client {
public:
	int issues;
	int wait;
	int serve;
	std::chrono::system_clock::time_point await;
};

class Basket {
public:
	int amount;
	double job;
	double wait;
	std::chrono::system_clock::time_point begin;
	bool finished;
};

class Store {
public:
	int registerAmount;
	double intensity;
	double speed;
	double avg;
	int len;

	std::list<Client> queue;
	std::vector<Basket> basket;
	std::vector<std::thread*> thread;

	Store(int count, double intensity, double speed, double avg, int len);
	void Reg(int name, Client cust, double speed);
	void Run();
	void Statistics();

	std::chrono::system_clock::time_point t;
	int clock;

	double worked;
	double waited;

	int serviced;
	int notServiced;

	double realAvg;
	double expect;
	double serving;
	double canceled;

};
