#pragma once
#ifndef INCLUDE_TASK_H
#define INCLUDE_TASK_H

#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <future>
#include <chrono>
#include <deque>
#include <vector>
#include <cmath>


class Client {
public:
	int count_products;
	int time_waiting;
	int time_service;
	std::chrono::system_clock::time_point stay_inqueue;
};

class Cash_register {
public:
	int number;
	double time_waiting_cr;
	double  time_service_cr;
	bool cashregister_is_free=false;
	std::chrono::system_clock::time_point in_queue;
};
class Shop {
public:
	int count_cash_registers;
	double intensity_client_flow;
	double speed_processing_goods_checkout;
	double average_count_products_cart;
	int max_len_queue;

	std::mutex mutexx;
	std::vector< std::thread* > pool;
	std::queue<Client> queue;
	std::vector<Cash_register> cash_register;

	int clients_served;
	int clients_unserved;
	double average_length_queue;
	double average_time_client_inqueue;
	double average_time_client_cassa;
	double average_service_time_cassa;
	double average_waiting_cassa;

	double probability_of_failure;
	double relative_store_throughput;
	double absolute_store_throughput;

	int ti_me;
	std::chrono::system_clock::time_point time_time;
	Shop(int count_cashreg, double intensiv_client, double speed, double avr, int maxlen);

	void start_work();
	void statistics();
	void cash_obrabotka(int num_ber, Client client, double speed);
};


#endif // INCLUDE_TASK_H
