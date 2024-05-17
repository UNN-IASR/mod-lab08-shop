#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <list>
#include <cmath>

class cli {
public:
	int issues;
	int expect;
	int serve;
	std::chrono::system_clock::time_point deque;
};

class cas {
public:
	int count;
	double job;
	double expect;
	std::chrono::system_clock::time_point begin;
	bool f;
};

class restore {
public:
	int cassa;
	double intens;
	double speed_servis;
	double average;
	int len;

	std::mutex mut;

	std::list<cli> que;
	std::vector<cas> cart;
	std::vector<std::thread*> thr;

	restore(int num, double intens, double _speed, double aver, int lenght);
	void cass(int name, cli cust, double speed_servis);
	void job();
	void stat();

	std::chrono::system_clock::time_point tim;
	int clock;

	int ready;
	int unready;

	double middle;
	double waits;
	double serves;
	double worked;
	double waited;

	double can;
	double skip;
	double miss;
};
