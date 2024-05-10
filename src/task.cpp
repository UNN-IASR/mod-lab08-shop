#include "../include/task.h"

Store::Store(int num, double intens, double _speed, double aver, int lenght) {
	cassa = num;
	intensive = intens;
	speed = _speed;
	average = aver;
	length = lenght;
	Time = 0;
	served = 0;
	unserved = 0;
	avera = 0;
	waiting = 0;
	serving = 0;

	que = std::list<Customer>();
	cart = std::vector<Cart>();
	thread = std::vector<std::thread*>();

	for (int i = 0; i < cassa; i++) {
		cart.push_back(Cart());
		cart[i].number = i;
		cart[i].work = 0;
		cart[i].wait = 0;
		cart[i].flag = true;
		cart[i].start = std::chrono::system_clock::now();
	}
}

void Store::cass(int name, Customer cust, double speed) {
	std::unique_lock<std::mutex> mute(mut);

	cart[name].wait += std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::system_clock::now() - cart[name].start).count();
	cart[name].work += cust.things / speed;
	cust.servis = (int)(cust.things / speed);

	std::this_thread::sleep_for(std::chrono::milliseconds((int)(cust.things / speed)));

	cart[name].start = std::chrono::system_clock::now();
	cart[name].flag = true;

	mute.unlock();
}

void Store::work() {
	tim = std::chrono::system_clock::now();

	srand(time(0));
	int time = 0;
	int next = rand() % 10 + 1;
	int nums = 0;

	while (true) {
		if (nums < intensive) {
			time++;
			if (time == next) {
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				nums++;
				Customer cust;
				cust.things = rand() % (2 * (int)average) + 1;
				cust.queque = std::chrono::system_clock::now();

				if (que.size() < length) {
					que.push_front(cust);
				}
				else {
					unserved++;
				}
				next = time + rand() % 10 + 1;
			}
		}

		for (int i = 0; i < cassa; i++) {
			if ((que.size() > 0) && (cart[i].flag == true)) {
				cart[i].flag = false;
				que.front().wait = std::chrono::duration_cast<std::chrono::milliseconds>
					(std::chrono::system_clock::now() - que.front().queque).count();

				thread.push_back(new std::thread(&Store::cass, this, i, que.front(), speed));

				avera += que.size();
				waiting += que.front().wait;
				serving += que.front().things / speed;
				served++;
				que.pop_front();
			}
		}
		if (que.empty() && nums == intensive)
			break;
	}
	for (auto& th : thread)
		th->join();

	Time = std::chrono::duration_cast<std::chrono::microseconds>
		(std::chrono::system_clock::now() - tim).count();
}

int fac(int n) {
	if (n == 0)
		return 1;
	else
		return n * fac(n - 1);
}

void Store::stat() {
	worked_cassa = 0;
	waited_cassa = 0;

	for (int i = 0; i < cassa; i++) {
		worked_cassa += cart[i].work;
		waited_cassa += cart[i].wait;
	}

	avera = avera / intensive;
	waiting = waiting / intensive;
	serving = serving / intensive;

	worked_cassa = worked_cassa / cassa;
	waited_cassa = waited_cassa / cassa;

	double l = intensive / Time;
	double m = (double)served / Time;
	double p = l / m;

	cancel = unserved / intensive;
	o_skip = 1.0 - cancel;
	a_skip = l * o_skip;

	double p0 = 1.0;
	for (int i = 0; i <= cassa; i++)
		p0 = p0 + pow(p, i) / fac(i);

	for (int i = cassa + 1; i < cassa + length; i++)
		p0 = p0 + pow(p, i) / (fac(cassa) * pow(cassa, i - cassa));

	p0 = 1.0 / p0;

	t_cancel = p0 * pow(p, (cassa + length)) / (double)(pow(cassa, length) * fac(cassa));
	t_o_skip = 1.0 - t_cancel;
	t_a_skip = l * t_o_skip;
}

