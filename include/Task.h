#pragma once
#include <iostream>
#include <mutex>
#include <queue>
#include <random>
#include <thread>
#include <vector>
#include <fstream>
#include <math.h>
#include <condition_variable>

using std::mutex;
using std::condition_variable;
using std::queue;
using std::vector;
using std::thread;
using std::unique_lock;
using std::lock_guard;
using std::default_random_engine;
using std::poisson_distribution;
using std::normal_distribution;
using std::ofstream;
using std::endl;

int fact(int n);

class Client {
public:
	Client(int items);
	int getItemsNum();
private:
	int num_items;
};

class task {
public:
	task(int _num_checkouts, double _lambda, int _checkout_time_ms,
		int _max_queue_length, int _mean_num_items);
	void startSimulation();
	void setClientsFinished(bool status);
	int clients_served = 0;
	int rejected_clients = 0;
	double total_time = 0.0;
	double idle_time = 0.0;
	vector<thread> checkout_threads;
	// int max_queue_length;

private:
	int num_checkouts;  // Êîëè÷åñòâî êàññ
	double lambda;  // Èíòåíñèâíîñòü ïîòîêà ïîêóïàòåëåé
	int checkout_time_ms;  // Ñêîðîñòü îáðàáîòêè òîâàðà íà êàññå (â ìèëëèñåêóíäàõ)
	int max_queue_length;   // Ìàêñèìàëüíàÿ äëèíà î÷åðåäè
	double mean_num_items;  // Ñðåäíåå êîëè÷åñòâî òîâàðîâ â òåëåæêå ïîêóïàòåëÿ
	queue<Client> client_queue;

	mutex queue_mutex;
	mutex idle_mutex;
	condition_variable queue_condition;
	condition_variable idle_condition;
	queue<int> idle_checkouts;
	vector<int> checkout_times;
	bool clients_finished;
	void serveClients(int id);
	void generateClients();
};
