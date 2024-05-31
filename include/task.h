#pragma once
#include <thread>
#include <mutex>
#include <cmath>
#include <vector>
#include <chrono>
#include <list>

class Client {
public:
	int issues;
	int wait;
	int serve;
	std::chrono::system_clock::time_point queue;
};

class Basket {
public:
	int num;
	double job;
	double wait;
	std::chrono::system_clock::time_point begin;
	bool f;
};

class Restore {
public:
	int cassa;
	double intens;
	double v;
	double middle;
	int len;

	std::mutex mut;

	std::list<Client> que;
	std::vector<Basket> basket;
	std::vector<std::thread*> thread;

	Restore(int count, double _intens, double _v, double _aver, int _lenght);
	void cass(int name, Client cust, double v);
	void job();
	void statisi();

	std::chrono::system_clock::time_point t;
	int clock;

	int servis;
	int not_servis;

	double aver;
	double expecting;
	double serving;
	double worked;
	double waited;

	double can;
	double o;
	double a;
};
