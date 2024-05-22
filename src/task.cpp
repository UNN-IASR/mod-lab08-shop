// Copyright 2024 MISha
#include "../include/task.h"
#include <thread>
#include <iostream>
#include <chrono>
#include <functional>
#include <queue>

Shop::Shop(int _numberCash, int _maxQueue, float _speedProgressing)
{
	numberCash = _numberCash;
	maxQueue = _maxQueue;
    speedProcessing = _speedProgressing;
    Buy = 0;
    NotBuy = 0;
    SumJopTimeCassa = 0;
    CountJopCassa = 0;
}
int Shop::NumbetThread = 0;
int Shop::speedProcessing = 0;
std::queue<int> Shop::clientQueue;
int Shop::SumJopTimeCassa=0;
int Shop::CountJopCassa=0;
void Shop::AddNewClient(int NumberProduct,int NameClient)
{
    std::cout << "Client "<<NameClient<<" Ïðèøåë êëèåíò\n";
    if (NumbetThread < numberCash&&clientQueue.size()==0)
    {
        std::cout << "Client " << NameClient << " Ñâîáîäíàÿ êàññà\n";
        MyThread myThread(NumberProduct,NameClient);
        NumbetThread++;
        Buy++;
        return;
    }
    if (NumbetThread == numberCash && clientQueue.size() >= 0 && clientQueue.size() < maxQueue)
    {
        std::cout << "Client " << NameClient << " Äîáàâèëè â î÷åðåäü\n";
        clientQueue.push(NumberProduct);
        SumLeightQueu.push_back(clientQueue.size());
        return;
    }
    std::cout << "Client " << NameClient << " Ïðîãîíÿåì êëèåíòà\n";
    NotBuy++;
}
void Shop::NextClient()
{
    if (clientQueue.size() > 0)
    {
        std::cout << "Âûòàùèëè èç î÷åðåäè\n";
        MyThread thread(clientQueue.front(),0);
        clientQueue.pop();
    }
}
MyThread::MyThread(int _numberProduct,int NameClient)
{
    NumberProduct = _numberProduct;
    std::thread thread(&MyThread::Start,this,_numberProduct,NameClient);
    thread.detach();
}
void MyThread::Start(int _numberProduct, int NameClient)
{
    Shop::SumJopTimeCassa += _numberProduct * Shop::speedProcessing;
    Shop::CountJopCassa++;
    std::cout << "Client " << NameClient << " Îáðàáîòêà\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(_numberProduct*Shop::speedProcessing));
    std::cout << "Client " << NameClient << " Îáñëóæåí\n";
    Shop::NumbetThread--;
    Shop::NextClient();
}


