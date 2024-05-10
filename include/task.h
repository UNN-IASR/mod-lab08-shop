#pragma once
#include <thread>
#include <string>
#include <mutex>
#include <future>
#include <cmath>
#include <queue>
#include <deque>
#include <vector>
#include <chrono>
#include <list>

class Customer {
public:
	int things;
	int wait;
	int servis;
	std::chrono::system_clock::time_point queque;
};

class Cart {
public:
	int number;
	double work;
	double wait;
	std::chrono::system_clock::time_point start;
	bool flag;
};

class Store {
public:
	int cassa;
	double intensive;
	double speed;
	double average;
	int length;

	std::mutex mut;

	std::list<Customer> que;
	std::vector<Cart> cart;
	std::vector<std::thread*> thread;

	Store(int num, double intens, double _speed, double aver, int lenght);
	void cass(int name, Customer cust, double speed);
	void work();
	void stat();

	std::chrono::system_clock::time_point tim;
	int Time;

	int served;
	int unserved;

	double avera;
	double waiting;
	double serving;
	double worked_cassa;
	double waited_cassa;

	double cancel;
	double o_skip;
	double a_skip;

	double t_cancel;
	double t_o_skip;
	double t_a_skip;
};
