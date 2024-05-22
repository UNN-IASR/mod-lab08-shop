// Copyright 2024 MISha
#pragma once
#include <vector>
#include <queue>
class Shop
{
public:
	int numberCash;
	int maxQueue;
	static int speedProcessing;
	static int NumbetThread;
	static std::queue<int> clientQueue;
	int Buy;
	int NotBuy;
	//static int AvgTimeQueue;
	static int SumJopTimeCassa;
	static int CountJopCassa;
	std::vector<int> SumLeightQueu;
	Shop(int _numberCash, int _maxQueue, float _speedProgressing);
	void AddNewClient(int NumberProduct,int NameClient);
	static void NextClient();
};
class MyThread
{
public:
	int NumberProduct;
	void Start(int _numberProduct,int NameClient);
	MyThread(int _numberProduct, int NameClient);



};
