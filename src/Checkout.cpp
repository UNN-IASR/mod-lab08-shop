#include "Checkout.h"

Checkout::Checkout(int id, milliseconds itemProcessDuration)
	: _id(id)
	, _itemProcessDuration(itemProcessDuration)
	, _shouldWork(true)
	, _workload(std::nullopt)
	, _totalOperationTime(std::chrono::seconds(0))
	, _actualWorkTime(std::chrono::seconds(0))
	, _thread([&] { workCycle(); })
	, _mutex() {}

Checkout::Checkout(const Checkout &checkout)
		: Checkout(checkout._id, checkout._itemProcessDuration) {}

Checkout::~Checkout()
{
	stopIfWorking();
}

void Checkout::serve(const shared_ptr<Customer> &customer)
{
	std::lock_guard<std::mutex> lock(_mutex);
	_workload = customer;
}

void Checkout::workCycle()
{
	auto begin = std::chrono::steady_clock::now();

	while (_shouldWork)
	{
		if (_workload.has_value())
		{
			auto workBegin = std::chrono::steady_clock::now();

			auto customer = _workload.value();
			std::this_thread::sleep_for(_itemProcessDuration * customer->getItemCount());

			std::lock_guard<std::mutex> lock(_mutex);
			_workload.reset();

			auto workEnd = std::chrono::steady_clock::now();

			_actualWorkTime += workEnd - workBegin;
		}

		std::this_thread::sleep_for(milliseconds(100));
	}

	auto end = std::chrono::steady_clock::now();

	_totalOperationTime = end - begin;
}

void Checkout::stopIfWorking()
{
	if (_shouldWork)
	{
		_shouldWork = false;
		_thread.join();
	}
}

bool Checkout::isBusy() const
{
	return _workload.has_value();
}

int Checkout::getId() const
{
	return _id;
}

duration<float> Checkout::getTotalOperationTime() const
{
	return _totalOperationTime;
}

duration<float> Checkout::getActualWorkTime() const
{
	return _actualWorkTime;
}
