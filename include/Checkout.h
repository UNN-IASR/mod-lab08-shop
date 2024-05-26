#pragma once

#include "Customer.h"
#include <chrono>
#include <thread>
#include <memory>
#include <optional>
#include <mutex>

using std::chrono::milliseconds;
using std::chrono::duration;
using std::shared_ptr;

class Checkout
{
private:
	int _id;
	milliseconds _itemProcessDuration;

	bool _shouldWork;
	std::optional<shared_ptr<Customer>> _workload;
	std::thread _thread;
	std::mutex _mutex;

	duration<float> _totalOperationTime;
	duration<float> _actualWorkTime;

	void workCycle();

public:
	Checkout(int id, milliseconds itemProcessDuration);
	Checkout(const Checkout& checkout);
	~Checkout();

	[[nodiscard]] bool isBusy() const;
	[[nodiscard]] int getId() const;
	[[nodiscard]] duration<float> getTotalOperationTime() const;
	[[nodiscard]] duration<float> getActualWorkTime() const;

	void serve(const shared_ptr<Customer> &customer);
	void stopIfWorking();
};
