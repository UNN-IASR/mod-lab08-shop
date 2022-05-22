#include "task.h"
#include <cmath>

//customer interfaces

customer::customer(unsigned int id, unsigned int cart_size) : id(id), cart_size(cart_size) {}
void customer::inqueued()
{
	start = std::chrono::system_clock::now();
}

std::chrono::milliseconds customer::serviced()
{
	end = std::chrono::system_clock::now();
	auto time = end - start;
	return std::chrono::duration_cast<std::chrono::milliseconds>(end-start);
}

unsigned int customer::get_id()
{
	return id;
}

unsigned int customer::get_cart()
{
	return cart_size;
}


//cashbox interfaces

cashbox::cashbox(shop* host, unsigned int id, std::chrono::milliseconds item_time) : host(host), item_time(item_time), id(id),
					work_start(std::chrono::system_clock::now()), work_end(std::chrono::system_clock::now()) {}

void cashbox::operator()()
{
	while (host->shop_available)
	{
		host->mut.lock();
		if (!host->clients.empty())
		{
			customer client = host->clients.front();
			host->clients.pop();
			host->mut.unlock();

			work_start = std::chrono::system_clock::now();
			host->mean_wait_time += std::chrono::duration_cast<std::chrono::milliseconds>(work_start - work_end);

			std::this_thread::sleep_for(item_time * client.get_cart());
			std::chrono::milliseconds service_time = client.serviced();
			host->serv_time += service_time.count();
			host->customers_serviced++;

			work_end = std::chrono::system_clock::now();
			host->mean_work_time += std::chrono::duration_cast<std::chrono::milliseconds>(work_end - work_start);

			std::cout << "Cashbox <" << id << "> serviced " << client.get_id() << "'s client with " << client.get_cart()<<" items\n Service time is: "<< service_time.count()<<"\n";

			continue;
		}
		host->mut.unlock();
	}
}


//shop interfaces

shop::shop(unsigned int queue_length, unsigned int cashboxes_count, std::chrono::milliseconds item_time) : queue_max_length(queue_length), shop_available(true)
{
	for (int i = 0; i < cashboxes_count; i++)
	{
		cashboxes.push_back(new cashbox(this, i, item_time));
		workers.push_back(std::thread(*(cashboxes[i])));
	}
}

void shop::service(customer client)
{
	mean_queue_length += clients.size();
	mut.lock();
	if (clients.size() < queue_max_length)
	{
		client.inqueued();
		clients.push(client);
	}
	else
	{
		customers_left++;
		std::cout << "Client <"<<client.get_id()<<"> left with "<<client.get_cart()<<" items\n";
	}
	mut.unlock();
}

void shop::close()
{
	if (shop_available)
	{
		while (!clients.empty())
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		shop_available = false;
		for (int i = 0; i < workers.size(); i++)
		{
			workers[i].join();
		}
	}
}

void shop::print_stat(double input_intencity)
{
	std::cout << "===============================\n";
	std::cout << "Statistics:\n";
	std::cout << "Total customers: " << customers_left + customers_serviced << "\n";
	std::cout << "Total customers serviced: " << customers_serviced << "\n";
	std::cout << "Total customers left: " << customers_left << "\n";
	std::cout << "Mean queue length: " << queue_max_length << "\n";
	std::cout << "Mean service time: " << serv_time / (customers_left + customers_serviced) << "\n";
	std::cout << "Mean work time: " << mean_work_time.count() / cashboxes.size() << "\n";
	std::cout << "Mean wait time: " << mean_wait_time.count() / cashboxes.size() << "\n";
	std::cout << "===============================\n";

	double service_intencity = (double)customers_serviced / (double)(mean_work_time.count() / cashboxes.size());
	double intencity = input_intencity / service_intencity;

	double decline_probability = (std::pow(intencity,queue_max_length + 1)*(1 - intencity)) / (1 - std::pow(intencity, queue_max_length + 2));
	std::cout << "Decline probability: " << decline_probability << "\n";
	std::cout << "Relative throughput: " << 1 - decline_probability <<"\n";
	std::cout << "Absolute throughput: " << input_intencity * (1 - decline_probability) << "\n";
}

unsigned int shop::get_queue_size()
{
	return clients.size();
}

shop::~shop()
{
	close();
}
