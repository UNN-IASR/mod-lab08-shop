#include "task.h"

Store::Store(int count, double intensity, double speed, double avg, int len) {
	this->registerAmount = count;
	this->intensity = intensity;
	this->speed = speed;
	this->avg = avg;
	this->len = len;
	clock = 0;
	serviced = 0;
	notServiced = 0;
	realAvg = 0;
	expect = 0;
	serving = 0;

	queue = std::list<Client>();
	basket = std::vector<Basket>();
	thread = std::vector<std::thread*>();

	for (int i = 0; i < registerAmount; i++) {
		basket.push_back(Basket());
		basket[i].amount = i;
		basket[i].job = 0;
		basket[i].wait = 0;
		basket[i].f = true;
		basket[i].begin = std::chrono::system_clock::now();
	}
}

void Store::Reg(int _name, Client _client, double _v) {
	std::unique_lock<std::mutex> mute(mut);

	basket[_name].wait += std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - basket[_name].begin).count();
	basket[_name].job += _client.issues / _v;
	_client.serve = (int)(_client.issues / _v);

	std::this_thread::sleep_for(std::chrono::milliseconds((int)(_client.issues / _v)));

	basket[_name].begin = std::chrono::system_clock::now();
	basket[_name].f = true;

	mute.unlock();
}

void Store::Run() {
	t = std::chrono::system_clock::now();

	srand(time(0));
	int time = 0;
	int news = rand() % 10 + 1;
	int nums = 0;

	while (true) {
		if (nums < intensity) {
			time++;
			if (time == news) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				nums++;
				Client cust;
				cust.issues = rand() % (2 * (int)avg) + 1;
				cust.await = std::chrono::system_clock::now();

				if (queue.size() < len) {
					queue.push_front(cust);
				}
				else {
					notServiced++;
				}
				news = time + rand() % 10 + 1;
			}
		}

		for (int i = 0; i < registerAmount; i++) {
			if ((queue.size() > 0) && (basket[i].f == true)) {
				basket[i].f = false;
				queue.front().wait = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - queue.front().await).count();

				thread.push_back(new std::thread(&Store::Reg, this, i, queue.front(), speed));

				realAvg += queue.size();
				expect += queue.front().wait;
				serving += queue.front().issues / speed;
				serviced++;
				queue.pop_front();
			}
		}
		if (queue.empty() && nums == intensity)
			break;
	}
	for (auto& th : thread)
		th->join();

	clock = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - t).count();
}

void Store::Statistics() {
	worked = 0;
	waited = 0;

	for (int i = 0; i < registerAmount; i++) {
		worked += basket[i].job;
		waited += basket[i].wait;
	}

	realAvg = realAvg / intensity;
	expect = expect / intensity;
	serving = serving / intensity;

	worked = worked / registerAmount;
	waited = waited / registerAmount;

	double l = intensity / clock;
	double m = (double)serviced / clock;
	double p = l / m;

	can = notServiced / intensity;
	o = 1.0 - can;
	a = l * o;
}
