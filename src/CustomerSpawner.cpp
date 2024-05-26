#include "CustomerSpawner.h"
#include <utility>
#include <cmath>

CustomerSpawner::CustomerSpawner(Handler handler, unsigned int seed, double expectedValue, double variance)
	: onCustomerSpawned(std::move(handler))
	, randomEngine(seed)
	, dist(getLeftBoundary(expectedValue, variance), getRightBoundary(expectedValue, variance)) {}

void CustomerSpawner::spawn()
{
	static int clientCount = 0;
	onCustomerSpawned(Customer(clientCount++, dist(randomEngine)));
}

double CustomerSpawner::getLeftBoundary(double expectedValue, double variance)
{
	return expectedValue - std::sqrt(3 * variance);
}

double CustomerSpawner::getRightBoundary(double expectedValue, double variance)
{
	return expectedValue + std::sqrt(3 * variance);
}
