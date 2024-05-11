#include "task.h"
#include <iostream>
#include <cmath>

Shop::Shop(int n1, double n2, double n3, int n4, int n5) {
    numberCass = n1; //количество касс
    intensityOfCustomerFlow = n2; //интенсивность потока покупателей
    speedProcessingCheckout = n3; //скорость обработки товара на кассе
    averageOfItemInCart = n4; // среднее количество товаров в тележке покупател€
    maxQueueLength = n5; //максимальна€ длина очереди

    queueCustomers = std::queue<Customer>();
    vectorCass = std::vector<Cassa>();
    threadCass = std::vector<std::thread*>();

    //запуск работы касс
    for (int i = 0; i < numberCass; i++) {
        Cassa cassa;
        vectorCass.push_back(cassa);
        vectorCass[i].timeStart = std::chrono::system_clock::now();
    }
}

Shop::~Shop() {
}

//Ќачинаетс€ моделирование очереди клиентов.
void Shop::Start() {
    allTimeStart = std::chrono::system_clock::now();
    for (int i = 0; i < 50; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        srand(time(NULL));
        int _random = 1 + rand() % 100;
        //формируем покупател€
        if (_random <= intensityOfCustomerFlow*100) {
            Customer customer;
            customer.countItems = rand() % 7 + averageOfItemInCart + 1;
            customer.timeStart = std::chrono::system_clock::now();
            //добавл€ем его в очередь или выгон€ем
            if (queueCustomers.size() < maxQueueLength) {
                queueCustomers.push(customer);  
            } else {
                unservedCustomers++;
            }
            //провер€ем, есть ли свободные кассы
            for (int j = 0; j < numberCass; j++) {
                if (vectorCass[j].isWork == false && queueCustomers.size() != 0) {
                    vectorCass[j].isWork = true;
                    queueCustomers.front().timeInQueue = std::chrono::duration_cast<std::chrono::milliseconds>
                        (std::chrono::system_clock::now() - queueCustomers.front().timeStart).count();
                    threadCass.push_back(new std::thread(&Shop::Cash, this, j,
                        queueCustomers.front(), speedProcessingCheckout*100, i));

                    averageTimeInQueue += queueCustomers.front().timeInQueue;
                    averageTimeInCass += (int)(queueCustomers.front().countItems / (speedProcessingCheckout * 100));
                    servedCustomers++;
                    queueCustomers.pop();
                }
            }  
        }
        averageLenghtQueue += queueCustomers.size();
    }
    for (auto& th : threadCass) th->join();
    allTime = std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - allTimeStart).count();

     

}
//ћоделирование работы кассы
void Shop::Cash(int i, Customer customer, double speed, int j) {
    std::unique_lock<std::mutex> mu(myMutex);
    vectorCass[i].timePause += std::chrono::duration_cast<std::chrono::milliseconds>
        (std::chrono::system_clock::now() - vectorCass[i].timeStart).count();
    vectorCass[i].timeWork += (int)(customer.countItems / speed);
    std::this_thread::sleep_for(std::chrono::milliseconds((int)(customer.countItems / speed)));
    vectorCass[i].timeStart = std::chrono::system_clock::now();
    vectorCass[i].isWork = false;
    mu.unlock();
} 

void Shop::Stata() {
    std::cout << "¬сего покупателей: " << servedCustomers + unservedCustomers << std::endl;
    std::cout << "»з них:" << std::endl;
    std::cout << "ќбслужены: " << servedCustomers << std::endl;
    std::cout << "”шедшие: " << unservedCustomers << std::endl;
    averageLenghtQueue = averageLenghtQueue / 50;
    std::cout << "—редн€€ длина очереди: " << averageLenghtQueue << std::endl;
    averageTimeInQueue = averageTimeInQueue / servedCustomers;
    averageTimeInCass = averageTimeInCass / servedCustomers;
    std::cout << "среднее врем€ нахождени€ в очереди: " << averageTimeInQueue << " ms" << std::endl;
    std::cout << "среднее врем€ нахождени€ на кассе: " << averageTimeInCass << " ms" << std::endl;

    for (int i = 0; i < numberCass; i++) {
        averageTimeWorkCass += vectorCass[i].timeWork;
        averageTimeSleepCass += vectorCass[i].timePause;
    }
    averageTimeWorkCass = averageTimeWorkCass / numberCass;
    averageTimeSleepCass = averageTimeSleepCass / numberCass;

    std::cout << "—реднее врем€ работы кассы: " << averageTimeWorkCass << " ms" << std::endl;
    std::cout << "—реднее врем€ просто€ кассы: " << averageTimeSleepCass << " ms" << std::endl;

    std::cout << "////////////Ќа основе накопленных данных////////////" << std::endl;

    std::cout << "¬еро€тность отказа: " << (double)unservedCustomers/ (double)(unservedCustomers + servedCustomers) << std::endl;
    std::cout << "ќтносительна€ пропускна€ способность магазина: " << (double)servedCustomers/ (double)(unservedCustomers + servedCustomers) << std::endl;
    std::cout << "јбсолютна€ пропускна€ способность магазина: " << (double)servedCustomers / (double)(unservedCustomers + servedCustomers)* (intensityOfCustomerFlow*100) << std::endl;

    std::cout << "////////////“еоретические подсчЄты////////////" << std::endl;

    //интенсивность нагрузки канала 
    double ro = (1/intensityOfCustomerFlow) / (1/speedProcessingCheckout);
    int m = maxQueueLength;
    int n = numberCass;

    std::cout << "»нтенсивность нагрузки канала: " << ro << std::endl;

    //¬еро€тность, что все каналы свободны
    double Po = 1;
    for (int i = 1; i <= n; i++) {
        Po += ((double)pow(ro, i) / (double)Factorial(i));
    }
    for (int i = 1; i <= m; i++) {
        Po += ((double)pow(ro, n+i) / pow(n, i)*(double)Factorial(n));
    }
    Po = pow(Po, -1);

    std::cout << "¬еро€тность, что все каналы свободны: " << Po << std::endl;

    double Prej = Po * ((double)pow(ro, n + m) / (double)(pow(n, m) * Factorial(n)));
    std::cout << "¬еро€тность отказа: " << Prej << std::endl;
    
    double Q = 1 - Prej;
    std::cout << "ќтносительна€ пропускна€ способность магазина: " << Q << std::endl;

    double A = (intensityOfCustomerFlow*100) * Q;
    std::cout << "јбсолютна€ пропускна€ способность магазина: " << A << std::endl;

    
    std::cout << "allTime: " << allTime << std::endl;

}

int Factorial(int i) {
    if (i <= 0)
        return 1;
    else
        return i * Factorial(i - 1);
}