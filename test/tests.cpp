// Copyright 2024 Shalnov_Yuri
#include <gtest/gtest.h>
#include <iostream>
#include "Task.h"

TEST(TestCaseName1, TestName1) {
	int num_checkouts = 0;
	double lambda = 0.2;
	int checkout_time_ms = 10;
	double mean_num_items = 3;
	int max_queue_length = 7;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	EXPECT_EQ(s.clients_served, 0);
}

TEST(TestCaseName2, TestName2) {

	int num_checkouts = 2;
	double lambda = 0.2;
	int checkout_time_ms = 10;
	double mean_num_items = 3;
	int max_queue_length = 5;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	bool flag = false;
	double temp = 0;
	int z = 0;
	int st = 0;
	double mu = s.clients_served / s.total_time;
	double ro = lambda / mu;
	for (int i = 0; i <= num_checkouts + max_queue_length; i++) {
		temp = temp + pow(ro, i) / (pow(num_checkouts, st) * fact(z));
		if (z < num_checkouts)z++;
		else
			st++;
	}

	double P_0 = 1 / (temp * 10);
	double P_rej = pow(ro, num_checkouts + max_queue_length) * P_0 / (pow(num_checkouts, max_queue_length) * fact(num_checkouts));
	if (P_0 < 1 && P_rej < 1)
		flag = true;

	EXPECT_TRUE(flag);
}
TEST(TestCaseName3, TestName3) {
	int num_checkouts = 2;
	double lambda = 0.2;
	int checkout_time_ms = 10;
	double mean_num_items = 3;
	int max_queue_length = 5;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	int sum = s.clients_served + s.rejected_clients;
	EXPECT_EQ(sum, 100);
}
TEST(TestCaseName4, TestName4) {
	int num_checkouts = 2;
	double lambda = 0.2;
	int checkout_time_ms = 10;
	double mean_num_items = 3;
	int max_queue_length = 100;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	int n = s.clients_served;
	EXPECT_EQ(n, 100);
}
TEST(TestCaseName5, TestName5) {
	int num_checkouts = 0;
	double lambda = 0.1;
	int checkout_time_ms = 100;
	double mean_num_items = 7;
	int max_queue_length = 7;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	int sum = s.clients_served;
	EXPECT_EQ(sum, 0);
}
TEST(TestCaseName6, TestName6) {
	int num_checkouts = 2;
	double lambda = 0.1;
	int checkout_time_ms = 100;
	double mean_num_items = 7;
	int max_queue_length = 7;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	bool flag = false;
	if (s.rejected_clients > 0)
		flag = true;
	EXPECT_TRUE(flag);
}
TEST(TestCaseName7, TestName7) {
	int num_checkouts = 5;
	double lambda = 0.1;
	int checkout_time_ms = 1;
	double mean_num_items = 1;
	int max_queue_length = 50;
	task s(num_checkouts, lambda, checkout_time_ms, max_queue_length, mean_num_items);
	s.startSimulation();
	EXPECT_EQ(s.rejected_clients, 0);
}
