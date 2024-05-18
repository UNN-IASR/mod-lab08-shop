#include <iostream>
#include <vector>
#include <thread>
#include <random>
#include <queue>
#include <chrono>
#include <ctime>
#include <mutex>
using namespace std;
class Client {
public:
	std::vector<int> products;
	int time_obslugivania;
	double time_v_ocheredi;
	Client(int product, int skorost_obrabotki_tovara, int time_v_ocheredi);
};
class Kassa {
public:
	std::queue<Client*> ochered_clientov;
	std::mutex mtx;
	bool polnaya = false;
	int totalCustomers = 0;
	std::chrono::time_point<std::chrono::steady_clock> startTime;
	std::chrono::time_point<std::chrono::steady_clock> endTime;
	//std::chrono::time_point<std::chrono::steady_clock> waittime;
	void obslugivanie(Client* client);
};
class Supermarket {
public:
	std::vector<Kassa>kassi;
	std::queue<Client*>clients;
	int polnoe_vremya_obslugivania = 0;
	int kol_obslug_clientov = 0;
	int vremya_ogidania = 0;
	int max_dlina_ocheredi;
	int kol_neobslug_clientov = 0;
	int intensivnost_potoka_pokupatelei;
	int srednee_kol_productov;
	int count_kass;
	int skorost_obrabotki_tovara;
	int totaltime = 0;
	int count_ocheredei = 0;
	int summa_ocheredi = 0;
	int count_klient = 0;
	int time_v_kasse = 0;
	int time_v_ocheredi = 0;
	double srednya_dlina_ocheredi;
	Supermarket(int count_kass, int intensivnost_potoka_pokupatelei, int skorost_obrabotki_tovara, int srednee_kol_productov, int max_dlina_ocheredi);
	void addclient(Client* client);
	void obrabativaem_ochered();
	void work();
	void statistika();
	double total_time;
        double total_time0;
	double ttime;
};
