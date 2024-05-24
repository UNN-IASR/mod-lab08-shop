#include "../include/task.h"

Shop::Shop(int count_cashreg, double intensiv_client, double speed, double avr, int maxlen) {
	count_cash_registers = count_cashreg;
	intensity_client_flow = intensiv_client;
	speed_processing_goods_checkout = speed;
	average_count_products_cart = avr;
	max_len_queue = maxlen;

	ti_me = 0;
	clients_served = 0;
	clients_unserved = 0;
	average_length_queue = 0;
	average_time_client_cassa = 0;
	average_time_client_inqueue = 0;


	pool = std::vector< std::thread*>();
	queue = std::queue<Client>();
	cash_register = std::vector<Cash_register>();

	for (int i = 0; i < count_cashreg; i++) {
		Cash_register cash_registers;
		cash_register.push_back(cash_registers);
		cash_register[i].number = i;
		cash_register[i].time_service_cr = 0;
		cash_register[i].time_waiting_cr = 0;
		cash_register[i].in_queue = std::chrono::system_clock::now();
	}
}

void Shop::cash_obrabotka(int num_ber, Client client, double speed) {
	std::unique_lock<std::mutex> mute(mutexx);
	cash_register[num_ber].time_waiting_cr += std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - cash_register[num_ber].in_queue).count();
	cash_register[num_ber].time_service_cr += client.count_products / speed;
	client.time_service = (int)client.count_products / speed;
	std::this_thread::sleep_for(std::chrono::milliseconds((int)(client.count_products / speed)));
	cash_register[num_ber].in_queue = std::chrono::system_clock::now();
	cash_register[num_ber].cashregister_is_free = false;
	mute.unlock();
}

void Shop::start_work() {
	time_time = std::chrono::system_clock::now();
	srand((unsigned)time(NULL));
	int time = 0;
	int random_next_time = rand() % 10 + 1;

	int k = 0;
	while (true)
	{
		if (k < intensity_client_flow) {
			time++;
			if (time == random_next_time) {
				std::this_thread::sleep_for(std::chrono::milliseconds(5));
				k++;
				Client client;
				client.count_products = rand() % (3 * (int)average_count_products_cart);
				client.stay_inqueue = std::chrono::system_clock::now();
				if (queue.size() < max_len_queue) {
					queue.push(client);
				}
				else {
					clients_unserved++;
				}
				random_next_time = time + rand() % 10 + 1;
			}
		}
		for (int i = 0; i < count_cash_registers; i++) {
			if ((queue.size() > 0) && (cash_register[i].cashregister_is_free == false )) {
				cash_register[i].cashregister_is_free = true;
				queue.front().time_waiting = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - queue.front().stay_inqueue).count();
				pool.push_back(new std::thread(&Shop::cash_obrabotka, this,
					i, queue.front(), speed_processing_goods_checkout));
				average_length_queue = queue.size();
				average_time_client_inqueue += queue.front().time_waiting;
				average_time_client_cassa += queue.front().count_products / speed_processing_goods_checkout;
				clients_served++;
				queue.pop();
			}
		}
		if (queue.empty() && k == intensity_client_flow)
			break;
	}
	for (auto& th : pool) th->join();
	ti_me = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - time_time).count();
}

long factorial(long n) {
	if (n == 0)
		return 1;
	else
		return n * factorial(n - 1);
}
void Shop::statistics() {
	average_service_time_cassa = 0;
	average_waiting_cassa = 0;
	std::cout << "Total cleints:" << clients_served + clients_unserved << std::endl;
	std::cout << "Served cleints:" << clients_served << std::endl;
	std::cout << "Unserved cleints:" << clients_unserved << std::endl;
	average_length_queue = average_length_queue / intensity_client_flow;
	std::cout << "Avarage length queue:" << average_length_queue << std::endl;
	double average_time_client_cassa_inqueue = ((double)average_time_client_cassa / clients_served +
		(double)average_time_client_inqueue / clients_served);
	std::cout << "Avarage time client in queue and on cash_registers:" << average_time_client_cassa_inqueue << std::endl;
	for (int i = 0; i < count_cash_registers; i++) {
		average_service_time_cassa += cash_register[i].time_service_cr;
		average_waiting_cassa += cash_register[i].time_waiting_cr;
	}
	std::cout << "Avarage time work cash_registers:" << average_service_time_cassa / count_cash_registers << std::endl;
	std::cout << "Avarage time waiting cash_registers:" << average_waiting_cassa / count_cash_registers << std::endl;

	double lambda = intensity_client_flow / ti_me;
	double m = (double)clients_served / ti_me;
	double p = lambda/m;

	double p0 = 1.0;
	for (int i = 0; i <= count_cash_registers; i++)
		p0 = p0 + (double)pow(p, i) / (double)factorial(i);

	for (int i = 1; i <= max_len_queue; i++)
		p0 = p0 + (double)pow(p, count_cash_registers + i) / (factorial(count_cash_registers) * (double)pow(count_cash_registers, i ));

	p0 = 1.0 / p0;

	probability_of_failure = p0 * (double)pow(p, (count_cash_registers + max_len_queue)) / (double)(pow(count_cash_registers, max_len_queue) 
		* factorial(count_cash_registers));
	relative_store_throughput = 1.0 - probability_of_failure;
	absolute_store_throughput = lambda * relative_store_throughput;

	std::cout << " Teoria" << std::endl;
	std::cout << "Probability of failure:" << probability_of_failure << std::endl;
	std::cout << "Relative store throughput:" << relative_store_throughput << std::endl;
	std::cout << "Absolute store throughput:" << absolute_store_throughput << std::endl;

	std::cout << "Practica" << std::endl;
	std::cout << "Probability of failure:" << (double)clients_unserved/(clients_served + clients_unserved) << std::endl;
	std::cout << "Relative store throughput:" << 1 - ((double)clients_unserved / (clients_served + clients_unserved)) << std::endl;
	std::cout << "Absolute store throughput:" << (double)clients_served/ti_me << std::endl;

}
