//Copyright Annenko Misha 2024
#include "../include/task.h"

//Пользователь
Shopper::Shopper(int amount, int numb) {
	amount_of_products = amount;
	shopper_number = numb;
    transferred_to_cash = false;
}

Shopper::Shopper() {
	shopper_number = 0;
	amount_of_products = 0;
	transferred_to_cash = false;
}

//Супермаркерт
Shopping_mall::Shopping_mall(int ServiceTime) {
	in_service = false;
	service_time = ServiceTime;
}

Shopping_mall::Shopping_mall() {
	in_service = false;
	service_time = 0;
}

void Shopping_mall::Push_back_working_time(double work) {
	working_time.push_back(work);
}

void Shopping_mall::Push_back_waiting_time(double wait) {
	waiting_time.push_back(wait);
}

void Shopping_mall::start_of_service() {
	in_service = true;
}

bool Shopping_mall::Check_empty_shoppers() {
	if (shoppers.empty()) {
		return true;
	} else {
		return false;
	}
}

void Shopping_mall::Pop_front_shoppers() {
	if (Check_empty_shoppers() == true) {
		throw std::runtime_error("Âåêòîð êëèåíòîâ ïóñò");
	} else if (Check_empty_shoppers() == false) {
		shoppers.erase(shoppers.begin());
	}
}

void Shopping_mall::Push_back_shopper(Shopper shopper) {
	shoppers.push_back(shopper);
	time_spent_on_shoppers.push_back({ std::chrono::high_resolution_clock::now(),
	std::chrono::high_resolution_clock::now() });
}

bool Shopping_mall::Check_empty_waiting_time() {
	if (waiting_time.empty()) {
		return true;
	} else {
		return false;
	}
}

std::vector<double> Shopping_mall::Return_waiting_time() {
	if (Check_empty_waiting_time() == true) {
		return std::vector<double>{0};
	} else {
		return waiting_time;
	}
}

void Shopping_mall::End() {
	end = true;
}

void Shopping_mall::shopper_add_times
(std::chrono::high_resolution_clock::time_point point2, int number) {
	time_spent_on_shoppers[number] = { time_spent_on_shoppers[number][0], point2 };
}

void Shopping_mall::generation() {
	Service = new std::thread([this] {servicing(*this); });
}

std::vector<double> Shopping_mall::duration_work_shopper() {
	std::vector<double> duration_work;
	int i = 0;
	while( i < time_spent_on_shoppers.size()) {
		duration_work.push_back((double)std::chrono::duration_cast
			<std::chrono::milliseconds>
			(time_spent_on_shoppers[i][1] -
				time_spent_on_shoppers[i][0]).count());
		i++;
	}
	return duration_work;
}

void servicing(Shopping_mall& mall) {

	Shopper shopper;
	int shopper_service = 0;

	std::chrono::time_point start_waiting =
		std::chrono::high_resolution_clock::now();
	std::chrono::time_point end_waiting =
		std::chrono::high_resolution_clock::now();
	bool launch_control = false;

	while (!mall.end) {

		if (mall.shoppers.size() != 0) {

			if (launch_control) {
				launch_control = false;
				end_waiting = std::chrono::high_resolution_clock::now();

				auto duration0 = std::chrono::duration_cast<std::chrono::milliseconds>(end_waiting - start_waiting);

				double buf0 = (double)duration0.count();
				mall.Push_back_waiting_time(buf0);
			}

			shopper = mall.shoppers.at(0);

			auto start = std::chrono::high_resolution_clock::now();

			mall.Pop_front_shoppers();
			mall.start_of_service();//begin servicing
			for (int i = 1; i <= shopper.amount_of_products; i++) {

				std::this_thread::sleep_for(std::chrono::milliseconds
				(mall.service_time));
			}

			mall.shopper_add_times
			(std::chrono::high_resolution_clock::now(),
				shopper_service);
			shopper_service++;

			auto stop = std::chrono::high_resolution_clock::now();
			auto shopper = std::chrono::duration_cast
				<std::chrono::milliseconds>(stop - start);
			double buf1 = (double)shopper.count();
			mall.Push_back_working_time(buf1);
		} else {
			if (launch_control) end_waiting = std::chrono::high_resolution_clock::now();
			else {
				start_waiting = std::chrono::high_resolution_clock::now();
				launch_control = true;
			}
		}
	}
}

int Cash_Choose(Shopping_mall array[], int money_count, int large_queue) {
	int num = -1;
	int k = 0;
	while (k < money_count) {
		if (!(array[k].in_service) &&
			!(array[k].shoppers.size() > 0) && num == -1) num = k;
		k++;
	}
	if (num == -1) {
		int queue = large_queue;
		int i = 0;
		while (i < money_count) {
			if (array[i].shoppers.size() < queue) {
				num = i; queue = array[i].shoppers.size();
			}
			i++;
		}
	}
	return num;
}

//Взял функцию у себя из 7 лаборторной работы
double Factorial_Recursive(int n) {
	//Рекурсивный вызов, при i != 0
	if (n != 0) return n * Factorial_Recursive(n - 1);
	//Базовый случай: факториал от 0 равен 1
	else return 1;
}

//double Degree(double num, int degree) {
	//if (degree == 0) return 1;
	//else if (degree > 0) return num * Degree(num, degree - 1);
	//else if (degree < 0) return 1 / Degree(num, -degree);
	
//}

double Degree(double num, int degree) {
    double result = 1.0;
    for (int i = 0; i < degree; ++i) {
        result *= num;
    }
    return result;
}

double Calculate_Average(std::vector<double> array) {
	double sum = std::accumulate(array.begin(), array.end(), 0.0);
	double count = std::distance(array.begin(), array.end());
	return sum / count;
}
