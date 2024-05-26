#include "Customer.h"
#include "CustomerSpawner.h"
#include "Shop.h"
#include "TimedCaller.h"
#include "Calculator.h"
#include <iostream>

int main()
{
	constexpr unsigned int checkoutCount = 2;
	constexpr double arrivalRate = 5;
	constexpr double serviceRate = 2;  // одного товара
	constexpr double avgItemCount = 5;
	constexpr unsigned int queueMaxLength = 5;

	constexpr auto workDuration = std::chrono::seconds(30);
	constexpr auto itemProcessDuration = std::chrono::duration_cast<milliseconds>(std::chrono::duration<double>(1 / serviceRate));
	constexpr auto timeout = std::chrono::duration_cast<milliseconds>(std::chrono::duration<double>(1 / arrivalRate));


	Shop shop(checkoutCount, itemProcessDuration, queueMaxLength);

	std::random_device randomDevice;
	CustomerSpawner spawner(
		[&shop](const Customer& customer) {
			shop.handleCustomer(std::make_shared<Customer>(customer));
		},
		randomDevice(),
		avgItemCount,
		3
	);

	TimedCaller caller{};
	caller.Call(
		[&spawner] { spawner.spawn(); },
		workDuration,
		timeout
	);

	shop.stopIfWorking();
	auto data = shop.getData();

	auto theoreticalStats = Calculator::calculateStats(
		arrivalRate,
		serviceRate * checkoutCount / avgItemCount,
		checkoutCount,
		queueMaxLength);
	std::cout << "\nTHEORY:\n"
	          << "rejection probability: " << theoreticalStats.rejectionProbability << '\n'
	          << "relative throughput: " << theoreticalStats.relativeThroughput << '\n'
	          << "absolute throughput: " << theoreticalStats.absoluteThroughput << '\n';

	std::cout << "\nDATA:\n"
	          << "rejected customer count: " << data.rejectedCustomerCount << '\n'
	          << "accepted customer count: " << data.acceptedCustomerCount << '\n'
	          << "avg queue length: " << (double) std::reduce(data.queueSizeSamples.begin(), data.queueSizeSamples.end()) / data.queueSizeSamples.size()  << '\n'
	          << "avg client service time (queue + checkout): " << (data.totalWaitTime / data.acceptedCustomerCount).count() << "s\n"
	          << "avg checkout work time: " << (data.actualWorkTimeAcrossCheckouts / checkoutCount).count() << "s\n"
	          << "avg checkout idle time: " << ((data.totalOperationTimeAcrossCheckouts - data.actualWorkTimeAcrossCheckouts) / checkoutCount).count() << "s\n";

	double actualArrivalRate = (double) (data.acceptedCustomerCount + data.rejectedCustomerCount) / workDuration.count();
	double actualRejectionProbability = (double) data.rejectedCustomerCount / ( data.acceptedCustomerCount + data.rejectedCustomerCount );
	double actualRelativeThroughput = 1 - actualRejectionProbability;
	double actualAbsoluteThroughput = actualArrivalRate * actualRelativeThroughput;

	std::cout << "\nACTUAL:\n"
	          << "rejection probability: " << actualRejectionProbability << '\n'
	          << "relative throughput: " << actualRelativeThroughput << '\n'
	          << "absolute throughput: " << actualAbsoluteThroughput << '\n';
}
