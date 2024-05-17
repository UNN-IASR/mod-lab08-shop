#include "task.h"

restore::restore(int num, double intens, double _speed, double aver, int lenght) {
	cassa = num;
	intens = intens;
	speed_servis = _speed;
	average = aver;
	len = lenght;
	clock = 0;
	ready = 0;
	unready = 0;
	middle = 0;
	waits = 0;
	serves = 0;

	que = std::list<cli>();
	cart = std::vector<cas>();
	thr = std::vector<std::thread*>();

	for (int i = 0; i < cassa; i++) {
		cart.push_back(cas());
		cart[i].count = i;
		cart[i].job = 0;
		cart[i].expect = 0;
		cart[i].f = true;
		cart[i].begin = std::chrono::system_clock::now();
	}
}

void restore::cass(int name, cli cust, double speed_servis) {
	std::unique_lock<std::mutex> mute(mut);

	cart[name].expect += std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - cart[name].begin).count();
	cart[name].job += cust.issues / speed_servis;
	cust.serve = (int)(cust.issues / speed_servis);

	std::this_thread::sleep_for(std::chrono::milliseconds((int)(cust.issues / speed_servis)));

	cart[name].begin = std::chrono::system_clock::now();
	cart[name].f = true;

	mute.unlock();
}

void restore::job() {

	tim = std::chrono::system_clock::now();

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
				cli cust;
				cust.issues = rand() % (2 * (int)average) + 1;
				cust.deque = std::chrono::system_clock::now();

				if (que.size() < len) {
					que.push_front(cust);
				}
				else {
					unready++;
				}
				news = time + rand() % 10 + 1;
			}
		}

		for (int i = 0; i < cassa; i++) {
			if ((que.size() > 0) && (cart[i].f == true)) {
				cart[i].f = false;
				que.front().expect = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - que.front().deque).count();

				thr.push_back(new std::thread(&restore::cass, this, i, que.front(), speed_servis));

				middle += que.size();
				waits += que.front().expect;
				serves += que.front().issues / speed_servis;
				ready++;
				que.pop_front();
			}
		}
		if (que.empty() && nums == intens)
			break;
	}
	for (auto& th : thr)
		th->join();

	clock = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - tim).count();
}

int fac(int n) {
	if (n == 0)
		return 1;
	else
		return n * fac(n - 1);
}

void restore::stat() {
	worked = 0;
	waited = 0;

	for (int i = 0; i < cassa; i++) {
		worked += cart[i].job;
		waited += cart[i].expect;
	}

	middle = middle / intens;
	waits = waits / intens;
	serves = serves / intens;

	worked = worked / cassa;
	waited = waited / cassa;

	double l = intens / clock;
	double m = (double)ready / clock;
	double p = l / m;

	can = unready / intens;
	skip = 1.0 - can;
	miss = l * skip;
}
