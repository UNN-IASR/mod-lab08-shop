#pragma once
#include <list>
#include <vector>
#include <queue>
#include <mutex>
#include <random>
#include <thread>
#include <condition_variable>
#include <iostream>


class Buyer {
public:
  std::vector<int> check;
  int number;
  explicit Buyer(std::vector<int> _check, int _number);
};

class Supermarket {
private:
  std::vector<std::thread*> threads;
  std::queue<Buyer*> buyerQueue;
  std::vector<std::queue<Buyer*>*> cashRegisterQueues;


  int cashRegisterAmount; //количество касс(каналов обслуживания)
  int flowRate; //интенсивность потока покупателей(заявки на обслуживание)
  int processingSpeed; //скорость обработки товара на кассе
  int averageNumberOfProducts; //среднее количество товаров в тележке покупателя
  int maxQueueLength; //максимальная длина очереди

  int maxPrice = 50;
  int maxBuyers;
  int countBuyers = 0; // покупатели

  int servedCustomers = 0;  //обслуженные покупатели
  double averageQueueLength = 0;  //средняя длина очереди
  double serviceTime = 0;  //среднее время нахождение покупателя в очереди + на кассе
  double averageCashRegisterWorkingTime;  //среднее время работы кассы
  double averageCashRegisterNotWorkingTime;  //среднее время простоя кассы

  int workingCashRegisterCount = 1;
  int refuseCount = 0;

  std::mutex myMutex;
public:
  Supermarket(int cashRegisterAmount, int flowRate, int processingSpeed, int averageNumberOfProducts, int maxQueueLength, int _maxBuyers);
  Buyer* createBuyer(int number);
  void run();
  void serveBuyer(Buyer* _buyer, int _number);
  void serveQueue(std::queue <Buyer*>* _buyers);
  void serveSupermarket();

  int getServedCustomers();
  int getNotServedCustomers();
  double getAverageQueueLength();
  double getServiceTime();
  double getAverageCashRegisterWorkingTime();
  double getAverageCashRegisterNotWorkingTime();
  double refuseProbability();
  double relativeBandwidth();
  double absoluteBandwidth();
  double factorial(double i);
};

