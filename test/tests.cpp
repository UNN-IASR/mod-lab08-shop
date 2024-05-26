// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include <gmock/gmock-matchers.h>
#include "TimedCaller.h"
#include "Math.h"
#include "Calculator.h"
#include "Customer.h"
#include "CustomerSpawner.h"
#include "Checkout.h"
#include "Shop.h"

using testing::Eq;
using testing::Ne;
using testing::Ge;
using testing::Le;
using testing::DoubleEq;
using testing::DoubleNear;
using testing::AnyOf;
using testing::AllOf;

//region TimedCaller Tests
TEST(TimedCallerTests, worksAsExpected)
{
	int i = 0;

	TimedCaller caller{};
	caller.Call(
		[&] { i++; },
		std::chrono::seconds(10),
		std::chrono::seconds(1)
	);

	EXPECT_THAT(i, Eq(10));
}
//endregion

//region Math Tests
TEST(MathTests, factorialWorksAsExpected)
{
	EXPECT_THAT(Math::factorial(0), Eq(1));
	EXPECT_THAT(Math::factorial(1), Eq(1));
	EXPECT_THAT(Math::factorial(8), Eq(40320));
}

TEST(MathTests, powWorksAsExpected)
{
	EXPECT_THAT(Math::pow(5u, 0), Eq(1));
	EXPECT_THAT(Math::pow(5u, 1), Eq(5));
	EXPECT_THAT(Math::pow(5u, 3), Eq(125));
}
//endregion

//region Calculator Tests
TEST(CalculatorTests, worksAsExpected)
{
	auto stats = Calculator::calculateStats(4, 2, 4, 4);

	double rejectionProbabilityByHand = (double) 1 / 183;

	EXPECT_THAT(stats.rejectionProbability, DoubleEq(rejectionProbabilityByHand));
	EXPECT_THAT(stats.relativeThroughput, DoubleEq(1 - rejectionProbabilityByHand));
	EXPECT_THAT(stats.absoluteThroughput, DoubleEq(4 * (1 - rejectionProbabilityByHand)));
}
//endregion

//region Customer Tests
TEST(CustomerTests, ctorCreatesExpectedObject)
{
	Customer customer(1, 5);

	EXPECT_THAT(customer.getId(), Eq(1));
	EXPECT_THAT(customer.getItemCount(), Eq(5));
}
//endregion

//region CustomerSpawner Tests
TEST(CustomerSpawnerTests, spawnWorksAsExpected)
{
	std::random_device randomDevice;

	int customerCount = 0;

	CustomerSpawner spawner(
		[&](const Customer& customer) {
			EXPECT_THAT(customer.getItemCount(), AllOf(Ge(2), Le(8)));
			EXPECT_THAT(customer.getId(), Eq(customerCount));
			customerCount++;
		},
		randomDevice(),
		5,
		3
	);

	spawner.spawn();
	spawner.spawn();
	spawner.spawn();
	spawner.spawn();
	spawner.spawn();

	EXPECT_THAT(customerCount, Eq(5));
}
//endregion

//region Checkout Tests
TEST(CheckoutTests, scenario1)
{
	Checkout checkout(1, milliseconds(500));
	auto operationBegin = std::chrono::steady_clock::now();
	EXPECT_THAT(checkout.getId(), Eq(1));
	EXPECT_THAT(checkout.isBusy(), Eq(false));

	std::this_thread::sleep_for(std::chrono::milliseconds(2500));

	auto workBegin = std::chrono::steady_clock::now();
	checkout.serve(std::make_shared<Customer>(Customer(1, 5)));
	EXPECT_THAT(checkout.isBusy(), Eq(true));

	std::this_thread::sleep_for(std::chrono::milliseconds(2750));

	EXPECT_THAT(checkout.isBusy(), Eq(false));
	auto workEnd = std::chrono::steady_clock::now();

	checkout.stopIfWorking();
	auto operationEnd = std::chrono::steady_clock::now();

	auto operationTime = std::chrono::duration_cast<duration<double>>(operationEnd - operationBegin);
	auto workTime = std::chrono::duration_cast<duration<double>>(workEnd - workBegin);

	EXPECT_THAT(operationTime.count(), DoubleNear(5.2, 0.2));
	EXPECT_THAT(workTime.count(), AllOf(Ge(2), Le(3)));
}
//endregion

//region Shop Tests
TEST(ShopTests, scenario1)
{
	Shop shop(2, milliseconds(500), 2);
	EXPECT_THAT(shop.isWorking(), Eq(true));

	shop.stopIfWorking();

	EXPECT_THAT(shop.isWorking(), Eq(false));

	auto data = shop.getData();

	EXPECT_THAT(data.rejectedCustomerCount, Eq(0));
	EXPECT_THAT(data.acceptedCustomerCount, Eq(0));
}

//TEST(ShopTests, scenario2)
//{
//	Shop shop(2, milliseconds(500), 2);
//
//	shop.handleCustomer(std::make_shared<Customer>(Customer(1, 5)));
//	std::this_thread::sleep_for(milliseconds(100));
//	shop.handleCustomer(std::make_shared<Customer>(Customer(2, 5)));
//	std::this_thread::sleep_for(milliseconds(100));
//	shop.handleCustomer(std::make_shared<Customer>(Customer(3, 5)));
//	std::this_thread::sleep_for(milliseconds(100));
//	shop.handleCustomer(std::make_shared<Customer>(Customer(4, 5)));
//	std::this_thread::sleep_for(milliseconds(100));
//	shop.handleCustomer(std::make_shared<Customer>(Customer(5, 5)));
//
//	shop.stopIfWorking();
//
//	auto data = shop.getData();
//
//	EXPECT_THAT(data.acceptedCustomerCount, Eq(4));
//	EXPECT_THAT(data.rejectedCustomerCount, Eq(1));
//	EXPECT_THAT((double) std::reduce(data.queueSizeSamples.begin(), data.queueSizeSamples.end()) / data.queueSizeSamples.size(), DoubleNear(1.8, 0.2));
//}
//endregion
