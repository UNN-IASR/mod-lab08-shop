#include "task.h"
#include <iostream>

double factorial(double n) {
    if (n == 0)
        return 1;
    else
        return n * factorial(n - 1);
}

Client::Client(int avgItems, int i) {
    tQueue = 0;
    tService = 0;
    Products = rand() % (2 * avgItems) + 1;
    id = i;
}

void Client::inQueue() {
    timestart = std::chrono::system_clock::now();
}

void Client::outQueue() {
    tQueue = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timestart).count();
}

void Client::outCashbox(int time) {
    tService = time;
    //this_thread::sleep_for(std::chrono::milliseconds(time));
}


Shop::Shop(int countcashbox, int requestrate, int servicespeed, int avgitems, int maxqueue) {

    countCashbox = countcashbox; //2
    requestRate = requestrate; //30
    serviceSpeed = servicespeed; //200
    avgItems = avgitems; //5
    maxQueue = maxqueue; //4


    completedClients = 0;
    rejectedClients = 0;

    cashboxes = std::vector<Cashbox>();
    queueClients = std::deque<Client*>();

    for (int i = 0; i < countCashbox; i++) {
        cashboxes.push_back(Cashbox());
        cashboxes[i].tWait = 0;
        cashboxes[i].tWork = 0;
        cashboxes[i].thrd = std::thread(&Shop::Start, this, i);
    }
}

void Shop::Start(int index) {
    while (completedClients + rejectedClients < requestRate) {
        m.lock();

        if (queueClients.size() > 0) {
            queueClients.at(0)->outQueue();

            int basket = queueClients.at(0)->Products;
            int id = queueClients.at(0)->id;

            cashboxes[index].tWork = cashboxes[index].tWork + (basket * serviceSpeed);

            queueClients.at(0)->outCashbox(basket * serviceSpeed);
            queueClients.pop_front();

            m.unlock();
            while (basket > 0) {
                //std::cout << " Klient nomer: " << id << ", pokupaet product nomer " << items << endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(serviceSpeed));
                basket -= 1;
            }
            completedClients++;
            std::cout << "Klient nomer " << id << " obsluzhen na kasse nomer " << index+1 << std::endl;

        } else {
            cashboxes[index].tWait = cashboxes[index].tWait + 10;
            m.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}

void Shop::Simulation() {
    srand(time(0));

    int countClients = 0;
    std::vector<Client> clients;

    for (int i = 0; i < requestRate; i++) {
        clients.push_back(Client(avgItems, i+1));
    }

    double avgQueueLen = 0.0;

    while (countClients < requestRate) {
        int time = rand() % 1000;
        std::this_thread::sleep_for(std::chrono::milliseconds(time));

        m.lock();
        avgQueueLen = avgQueueLen + queueClients.size();

        if (queueClients.size() < maxQueue)
        {
            queueClients.push_back(&clients.at(countClients));
            clients.at(countClients).inQueue();
            m.unlock();
        } else {
            rejectedClients++;
            std::cout << "Klient nomer " << clients.at(countClients).id << " ne obsluzhen" << std::endl;

            m.unlock();
        }
        countClients++;
    }

    //statistica
    double aWait = 0.0;
    double aWork = 0.0;
    double aTimeinqueue = 0.0;
    double aTimeincashbox = 0.0;
    double workTime = 0.0;

    for (int i = 0; i < countCashbox; i++) {
        cashboxes[i].thrd.join();
        aWait = aWait + cashboxes[i].tWait;
        aWork = aWork + cashboxes[i].tWork;
    }

    for (int i = 0; i < requestRate; i++) {
        aTimeinqueue = aTimeinqueue + clients.at(i).tQueue;
        aTimeincashbox = aTimeincashbox + clients.at(i).tService;
    }

    avgQueueLen= avgQueueLen / (double)countClients;
    workTime = aWork + aWait;
    aWait = aWait / (double)countCashbox;
    aWork = aWork / (double)countCashbox;
    aTimeinqueue = aTimeinqueue / (double)completedClients;
    aTimeincashbox = aTimeincashbox / (double)completedClients;

    std::cout << "____________________________________" << std::endl;

    std::cout << "Obsluzheno klientov: " << std::to_string(completedClients) << std::endl;
    std::cout << "Ne obsluzheno klientov: " << std::to_string(rejectedClients) << std::endl;
    std::cout << "Srednyaya dlina ocheredi: " << std::to_string(avgQueueLen) << std::endl;
    std::cout << "Srednee vremya nahozhdeniya v ocheredi i na kasse: " << std::to_string(aTimeinqueue/1000) << " i " << std::to_string(aTimeincashbox/1000) << std::endl;
    std::cout << "Srednee vremya raboty kassy: " << std::to_string(aWork/1000) << std::endl;
    std::cout << "Srednee vremya prostoya kassy: " << std::to_string(aWait/1000) << std::endl;


    //double la = requestRate / (workTime / 1000);
    //double mu = aTimeincashbox;
    double la = 4.0;
    double mu = 2.0;
    double p = la / mu;

    double P0 = 1.0;
    for (int i = 1; i <= countCashbox; i++)
        P0 = P0 + pow(p, i) / factorial(i);

    for (int i = countCashbox + 1; i < countCashbox + maxQueue; i++)
        P0 = P0 + pow(p, i) / (factorial(countCashbox) * pow(countCashbox, i - countCashbox));

    P0 = 1.0 / P0;
    double P = P0 * pow(p, (countCashbox + maxQueue)) / (double)(pow(countCashbox, maxQueue) * factorial(countCashbox));
    double Q = 1.0 - P;
    double A = la * Q;
    std::cout << "____________________________________" << std::endl;
    std::cout << "p = " << std::to_string(p) << std::endl;
    std::cout << "P0 = " << std::to_string(P0) << std::endl;
    std::cout << "____________________________________" << std::endl;

    std::cout << "Veroyatnost otkaza: " << std::to_string(P) << std::endl;
    std::cout << "Otnositelnaya propusknaya sposobnost: " << std::to_string(Q) << std::endl;
    std::cout << "Absolyutnaya propusknaya sposobnost: " << std::to_string(A) << std::endl;
}
