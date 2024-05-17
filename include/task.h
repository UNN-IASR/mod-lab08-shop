#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <list>
#include <cmath>

class Cli{
public:
	int issues;
	int expect;
	int serve;
	std::chrono::system_clock::time_point deque;
};

class Basket{
public:
	int count;
	double process;
	double keep;
	std::chrono::system_clock::time_point begin;
	bool f;
};

class Restore {
public:
	int num;
	double intens;
	double v;
	double aver;
	int len;

	std::mutex mute;

	std::list<Cli> queue;
	std::vector<Basket> basket;
	std::vector<std::thread*> thr;

	Restore(int _num, double _intens, double _v, double _aver, int _len);
	void cassa(int name, Cli cli, double speed);
	void job();
	void stats();

	std::chrono::system_clock::time_point clock;
	int watch;

	int ready;
	int unready;

	double middle;
	double expecting;
	double servis;
	double worked;
	double waited;

	double can;
	double skip;
	double miss;
};
