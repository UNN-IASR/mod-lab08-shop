#include "task.h"

using namespace std;

Customer::Customer()
{
	start = std::chrono::system_clock::now();
}

int Customer::wait()
{
	std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	int res = chrono::duration_cast<chrono::milliseconds>(now - start).count();
	return res;
}

int factor(int x)
{
	if (x <= 0) return 1;
	return x * factor(x - 1);
}

double myPow(double x, int n)
{
	double res = 1;
	for (int i = 0; i < n; i++)
		res = res * x;
	return res;
}
Shop::Shop(int count, int cust_count, int Time, int buy_count, int length)
{
	this->count = count;
	this->cust_count = cust_count;
	this->Time = Time;
	this->length = length;
	this->buy_count = buy_count;
	for (int i = 0; i < count; ++i)
	{
		threads.push_back(thread(&Shop::Work, this));
		threads.at(i).detach();
	}
}
void Shop::Work()
{
	srand(time(0));
	while (rejectedCount + processedCount < cust_count)
	{
		mut.lock();
		if (lenQueue > 0)
		{
			lenQueue--;
			wait_time += customers.front().wait();
			mut.unlock();
		}
		else
		{
			downtime += 1;
			mut.unlock();
			continue;
		}
		int count_buy = rand() % (buy_count / 2) + buy_count;
		for (int i = 0; i < count_buy; ++i)
		{
			this_thread::sleep_for(std::chrono::milliseconds(Time));
		}
		mut.lock();
		work_time += (double)count_buy * Time;
		buy_time += (double)count_buy * Time;
		cout << "Custumer number " << rejectedCount + processedCount + 1 << " served" << endl;
		processedCount++;
		mut.unlock();
	}
}

void Shop::Start_working(bool without_debug) {
	while (requestCount < cust_count)
	{
		requestCount++;
		this_thread::sleep_for(std::chrono::milliseconds(400));
		mut.lock();
		if (lenQueue > length)
		{
			cout << "Custumer number " << rejectedCount + processedCount + 1 << " rejected" << endl;
			rejectedCount++;
		}
		else
		{
			Customer cust = Customer();
			customers.push_back(cust);
			lenQueue++;
		}
		mut.unlock();
	}
	while (rejectedCount + processedCount < cust_count) {}
	wait_time = wait_time / processedCount;
	buy_time = buy_time / processedCount;
	downtime = downtime / count;
	work_time = work_time / count;
	double lambda = cust_count * 1000 / work_time;
	double nu = (double)1000 / buy_time;
	double ro = lambda / nu;
	double P0 = 1.;
	for (int i = 1; i <= count; ++i)
	{
		P0 += myPow(ro, i) / factor(i);
	}
	for (int i = count + 1; i < count + length; ++i)
	{
		P0 += myPow(ro, i) / (factor(count) * myPow(count, i - count));
	}
	P0 = 1. / P0;
	Prej = myPow(ro, count + length) * P0 / (myPow(count, length) * factor(count));
	Q = 1. - Prej;
	A = lambda * Q;
}

void Shop::Start_working()
{
	while (requestCount < cust_count)
	{
		requestCount++;
		this_thread::sleep_for(std::chrono::milliseconds(400));
		mut.lock();
		if (lenQueue > length)
		{
			cout << "Custumer number " << rejectedCount + processedCount + 1 << " rejected" << endl;
			rejectedCount++;
		}
		else
		{
			Customer cust = Customer();
			customers.push_back(cust);
			lenQueue++;
		}
		mut.unlock();
	}
	while (rejectedCount + processedCount < cust_count) {}
	wait_time = wait_time / processedCount;
	buy_time = buy_time / processedCount;
	downtime = downtime / count;
	work_time = work_time / count;
	cout << "Served: " << processedCount << endl;
	cout << "Rejection: " << rejectedCount << endl;
	cout << "Average waiting in queue: " << wait_time << endl;
	cout << "Average service at the checkout: " << buy_time << endl;
	cout << "Average checkout hours: " << work_time << endl;
	cout << "Average checkout downtime: " << downtime << endl;
	double lambda = cust_count * 1000 / work_time;
	double nu = (double)1000 / buy_time;
	double ro = lambda / nu;
	double P0 = 1.;
	for (int i = 1; i <= count; ++i)
	{
		P0 += myPow(ro, i) / factor(i);
	}
	for (int i = count + 1; i < count + length; ++i)
	{
		P0 += myPow(ro, i) / (factor(count) * myPow(count, i - count));
	}
	P0 = 1. / P0;
	double Prej = myPow(ro, count + length) * P0 / (myPow(count, length) * factor(count));
	cout << "Probability of failure: " << Prej << endl;
	double Q = 1. - Prej;
	cout << "Relative bandwidth: " << Q << endl;
	double A = lambda * Q;
	cout << "Absolute bandwidth: " << A << endl;
}
