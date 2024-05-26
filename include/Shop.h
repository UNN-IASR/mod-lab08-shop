#pragma once

#include "Customer.h"
#include "Checkout.h"
#include <memory>
#include <queue>
#include <vector>
#include <mutex>
#include <chrono>
#include <map>

using std::shared_ptr;
using std::unique_ptr;
using std::chrono::duration;
using std::chrono::seconds;

struct CollectedData
{
	unsigned int rejectedCustomerCount = 0;
	unsigned int acceptedCustomerCount = 0;
	duration<float> totalWaitTime = seconds(0);  // суммарное время ожидания (очередь + касса) всех клиентов
	duration<float> actualWorkTimeAcrossCheckouts = seconds(0);
	duration<float> totalOperationTimeAcrossCheckouts = seconds(0);
	std::vector<unsigned int> queueSizeSamples;
};

class Shop
{
private:
	std::mutex _handlerMutex;
	std::mutex _workMutex;
	bool _shouldWork;
	bool _shouldStart;
	std::thread _workThread;
	std::vector<shared_ptr<Checkout>> _checkouts;
	std::queue<shared_ptr<Customer>> _queue;
	unsigned int _queueMaxLength;
	CollectedData _data;
	std::map<int, std::chrono::time_point<std::chrono::steady_clock>> _customerIncomeTimeTable;
	milliseconds _checkoutItemProcessDuration;

	void workCycle();
	std::optional<shared_ptr<Checkout>> findFreeCheckout();

	void calculateAndSaveWaitTimeFor(const shared_ptr<Customer>& customer);

	/**
     * Предпринимает попытку добавить очередного клиента в очередь.
     * Если не достигнут %_queueMaxLength, клиент добавляется в очередь;
     * иначе нет.
     * @param customer Клиент для обслуживания
     * @return true, если добавить в очередь удалось; иначе false.
    */
	bool tryEnqueue(const shared_ptr<Customer>& customer);

public:
	Shop(unsigned int checkoutCount,
		 milliseconds checkoutItemProcessDuration,
		 unsigned int queueMaxLength);
	~Shop();

	void handleCustomer(const shared_ptr<Customer>& newCustomer);
	[[nodiscard]] CollectedData getData() const;
	void stopIfWorking();
	[[nodiscard]] bool isWorking() const;
};
