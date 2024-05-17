#include "../include/task.h"

Restore::Restore(int _num, double _intens, double _v, double _aver, int _len) {
	num = _num;
	intens = _intens;
	v = _v;
	aver = _aver;
	len = _len;
	watch = 0;
	ready = 0;
	unready = 0;
	middle = 0;
	expecting = 0;
	servis = 0;

	queue = std::list<Cli>();
	basket = std::vector<Basket>();
	thr = std::vector<std::thread*>();

	for (int i = 0; i < num; i++) {
		basket.push_back(Basket());
		basket[i].count = i;
		basket[i].process = 0;
		basket[i].keep = 0;
		basket[i].f = true;
		basket[i].begin = std::chrono::system_clock::now();
	}
}

void Restore::cassa(int name, Cli cli, double speed) {
	std::unique_lock<std::mutex> mut(mute);

	basket[name].keep += std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - basket[name].begin).count();
	basket[name].process += cli.issues / v;
	cli.serve = (int)(cli.issues / v);

	std::this_thread::sleep_for(std::chrono::milliseconds((int)(cli.issues / v)));

	basket[name].begin = std::chrono::system_clock::now();
	basket[name].f = true;

	mut.unlock();
}

void Restore::job() {
	clock = std::chrono::system_clock::now();

	srand(time(0));
	int time = 0;
	int news = rand() % 10 + 1;
	int cou = 0;

	while (true) {
		if cou < intens) {
			time++;
			if (time == news) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				cou++;
				Cli cli;
				cli.issues = rand() % (2 * (int)aver) + 1;
				cli.deque = std::chrono::system_clock::now();

				if (queue.size() < len) {
					queue.push_front(cli);
				}
				else {
					unready++;
				}
				news = time + rand() % 10 + 1;
			}
		}

		for (int i = 0; i < num; i++) {
			if ((queue.size() > 0) && (basket[i].f == true)) {
				basket[i].f = false;
				queue.front().keep = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - queue.front().deque).count();

				thr.push_back(new std::thread(&Restore::cassa, this, i, queue.front(), v));

				middle += queue.size();
				expecting += queue.front().keep;
				servis += queue.front().issues / v;
				ready++;
				queue.pop_front();
			}
		}
		if (queue.empty() && cou == intens)
			break;
	}
	for (auto& th : thr)
		th->join();

	watch = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - clock).count();
}

void Restore::stats() {
	worked = 0;
	waited = 0;

	for (int i = 0; i < num; i++) {
		worked += basket[i].process;
		waited += basket[i].expect;
	}

	middle = middle / intens;
	expecting = expecting / intens;
	servis = servis / intens;

	worked = worked / num;
	waited = waited / num;

	double o = intens / watch;
	double x = (double)ready / watch;
	double z = o / x;

	can = unready / intens;
	skip = 1.0 - can;
	miss = l * skip;
}
