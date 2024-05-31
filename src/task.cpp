#include "../include/task.h"

Restore::Restore(int count, double _intens, double _v, double _aver, int _lenght) {
	cassa = count;
	intens = _intens;
	v = _v;
	middle = _aver;
	len = _lenght;
	clock = 0;
	servis = 0;
	not_servis = 0;
	aver = 0;
	expecting = 0;
	serving = 0;

	que = std::list<Client>();
	basket = std::vector<Basket>();
	thread = std::vector<std::thread*>();

	for (int i = 0; i < cassa; i++) {
		basket.push_back(Basket());
		basket[i].num = i;
		basket[i].job = 0;
		basket[i].wait = 0;
		basket[i].f = true;
		basket[i].begin = std::chrono::system_clock::now();
	}
}

void Restore::cass(int _name, Client _client, double _v) {
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

void Restore::job() {
	t = std::chrono::system_clock::now();

	srand(time(0));
	int time = 0;
	int news = rand() % 10 + 1;
	int nums = 0;

	while (true) {
		if (nums < intens) {
			time++;
			if (time == news) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				nums++;
				Client cust;
				cust.issues = rand() % (2 * (int)middle) + 1;
				cust.queue = std::chrono::system_clock::now();

				if (que.size() < len) {
					que.push_front(cust);
				}
				else {
					not_servis++;
				}
				news = time + rand() % 10 + 1;
			}
		}

		for (int i = 0; i < cassa; i++) {
			if ((que.size() > 0) && (basket[i].f == true)) {
				basket[i].f = false;
				que.front().wait = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - que.front().queue).count();

				thread.push_back(new std::thread(&Restore::cass, this, i, que.front(), v));

				aver += que.size();
				expecting += que.front().wait;
				serving += que.front().issues / v;
				servis++;
				que.pop_front();
			}
		}
		if (que.empty() && nums == intens)
			break;
	}
	for (auto& th : thread)
		th->join();

	clock = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - t).count();
}

void Restore::statisi() {
	worked = 0;
	waited = 0;

	for (int i = 0; i < cassa; i++) {
		worked += basket[i].job;
		waited += basket[i].wait;
	}

	aver = aver / intens;
	expecting = expecting / intens;
	serving = serving / intens;

	worked = worked / cassa;
	waited = waited / cassa;

	double l = intens / clock;
	double m = (double)servis / clock;
	double p = l / m;

	can = not_servis / intens;
	o = 1.0 - can;
	a = l * o;
}

