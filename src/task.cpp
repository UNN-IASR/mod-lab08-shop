#include "task.h"
Client::Client(int product, int skorost_obrabotki_tovara, int time_v_ocheredi)
{
    int start = 1;
    int end = product;
    int x = rand() % (end - start + 1) + start;
    int countproduct = x;
    for (int i = 0; i < countproduct; ++i)
    {
        products.push_back(1);
    }
    time_obslugivania = (product / skorost_obrabotki_tovara);
    this->time_v_ocheredi = time_v_ocheredi;
}
void Kassa::obslugivanie(Client* client)
{
    std::lock_guard<std::mutex> lock(mtx);
    startTime = std::chrono::steady_clock::now();
    polnaya = true;
    std::this_thread::sleep_for(std::chrono::seconds(client->time_obslugivania));
    endTime = std::chrono::steady_clock::now();
    polnaya = false;
    endTime = std::chrono::steady_clock::now();
    //waittime = endTime;
    
}
Supermarket::Supermarket(int count_kass, int intensivnost_potoka_pokupatelei, int skorost_obrabotki_tovara, int srednee_kol_productov, int max_dlina_ocheredi):kassi(count_kass) {
    cout << "hhh" <<std::endl;
    this->count_kass = count_kass;
    this->intensivnost_potoka_pokupatelei = intensivnost_potoka_pokupatelei;
    this->skorost_obrabotki_tovara = skorost_obrabotki_tovara;
    this->srednee_kol_productov = srednee_kol_productov;
    this->max_dlina_ocheredi = max_dlina_ocheredi;
}
void Supermarket::addclient(Client* client)
{
    if (clients.size() <= max_dlina_ocheredi+count_kass) {
        clients.push(client);
        
        kol_obslug_clientov++;
        time_v_kasse += client->time_obslugivania;
        polnoe_vremya_obslugivania += client->time_obslugivania;
        summa_ocheredi++;
        vremya_ogidania++;
    }
    else {
        delete client;
        
        kol_neobslug_clientov++;
        count_klient--;
    }
}
void Supermarket::obrabativaem_ochered() {
    for (auto& kassa : kassi)
    {
        if (!clients.empty() && !kassa.polnaya) {
            Client* client = clients.front();
            clients.pop();
            kassa.obslugivanie(client);
        }
    }
}
void Supermarket::work() {

    std::random_device rd;
    std::mt19937 gen(rd());
    std::exponential_distribution<> arrivalTime(intensivnost_potoka_pokupatelei);
    auto start = std::chrono::high_resolution_clock::now();
    int time = 0;
    int clien = 0;
    for (int i=0;i<7;i++)
    {
        int numClients = intensivnost_potoka_pokupatelei+5;
        for (int i = 0; i < numClients; ++i) {
            Client* newclient = new Client(srednee_kol_productov, skorost_obrabotki_tovara, (time+1)*2);
            addclient(newclient);
            std::this_thread::sleep_for(5ms);
            count_klient++;
        }
        std::vector<std::thread> threads;
        while (clients.size() != 0) {
            for (auto& kassa : kassi) {
                if (!clients.empty() && !kassa.polnaya) {
                    Client* client = clients.front();
                    clients.pop();
                    threads.push_back(std::thread([&kassa, client, &time]() {
                        kassa.obslugivanie(client);
                        }));
                    kassa.polnaya = false;
                    auto end0 = std::chrono::high_resolution_clock::now();
                    total_time0 = (double)(end0 - start).count() / 1000000000;
                    time_v_ocheredi += total_time0 - client->time_v_ocheredi;
                }
                else if (clients.empty() && !kassa.polnaya) {
                    vremya_ogidania += 1;
                    cout << "ddddh" << endl;
                }
            }
        }
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        time++;
        count_ocheredei++;
        i++;
        auto end = std::chrono::high_resolution_clock::now();
        total_time = (double)(end - start).count() / 1000000000;
    }
}
void Supermarket::statistika() {
    std::cout << "gggg" << std::endl;
    std::cout << "Neobslugennie pokupateli: " << kol_neobslug_clientov << std::endl;
    
    std::cout << "Obslugennii pokupateli " << kol_obslug_clientov << std::endl;
    
    srednya_dlina_ocheredi = (double)(summa_ocheredi-count_kass) / kol_obslug_clientov;
    std::cout << "Srednya dlina ocheredi " << srednya_dlina_ocheredi << std::endl;
    
    double srednee_vremy_pokupki = (double)time_v_kasse / count_klient;
    double srednee_vremy_v_ocheredi = (double)time_v_ocheredi/ count_klient;
    double srednee_vremy_vezde = srednee_vremy_pokupki + srednee_vremy_v_ocheredi;
    std::cout << "Srednee vremya vezde " << srednee_vremy_vezde << std::endl;


    double t = 0;
    for (auto& kassa : kassi) {
        t += std::chrono::duration_cast<std::chrono::milliseconds>(kassa.endTime - kassa.startTime).count() / 1000;
    }
    double obslug = static_cast<double>(t) / 1000.0;
    double averageWorkTimePerCassa = obslug / count_kass;
    double vremy_raboti_kassi=(double)polnoe_vremya_obslugivania/count_kass;
    std::cout << "Srednee vremya raboti kassi " << vremy_raboti_kassi << std::endl;

    double srednee_vremya_ogidania = (double)vremya_ogidania / count_kass;
    std::cout << "Srednee vremya ogidania kassi " << srednee_vremya_ogidania << std::endl;

    double all_clients = kol_obslug_clientov + kol_neobslug_clientov;
    double veroyatnost_otkaza = (double)kol_neobslug_clientov / all_clients;
    std::cout << "Veroyatnost otkaza " << veroyatnost_otkaza << std::endl;

    double otnositelnaya_propusknaya_sposobnost = 1 - veroyatnost_otkaza;
    std::cout << "Otnositelnaya propusknaya sposobnost " << otnositelnaya_propusknaya_sposobnost<< std::endl;

    //double absolt_propusk_sposobnost = otnositelnaya_propusknaya_sposobnost * intensivnost_potoka_pokupatelei;
    std::cout << "Absolutnaya propusknaya sposobnost " << kol_obslug_clientov/total_time<< std::endl;

    std::cout << "total time: " << total_time << std::endl;
    double rho = (double)intensivnost_potoka_pokupatelei / skorost_obrabotki_tovara;
}
