#pragma once

#include "Customer.h"
#include <functional>
#include <random>

using Handler = std::function<void(const Customer&)>;

class CustomerSpawner
{
private:
	Handler onCustomerSpawned;
	std::default_random_engine randomEngine;
	std::uniform_int_distribution<int> dist;

	static double getLeftBoundary(double expectedValue, double variance);
	static double getRightBoundary(double expectedValue, double variance);

public:
	CustomerSpawner(Handler handler, unsigned int seed, double expectedValue, double variance);
	void spawn();
};
