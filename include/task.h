//Copyright Annenko Misha 2024
#pragma once
#include <iostream>
#include <chrono>
#include <vector>
#include <thread>
#include <numeric> // Äëÿ std::accumulate
#include <iterator> // Äëÿ std::distance

//Покупатель
class Shopper {
public:
    int shopper_number;
    int amount_of_products;
    bool transferred_to_cash;
    Shopper();
    Shopper(int numb, int amount);
};

//Супермаркерт
class Shopping_mall {
private:
    std::thread* Service;


    std::vector<std::vector
        <std::chrono::high_resolution_clock::time_point> > 
        time_spent_on_shoppers;



    std::vector<double> waiting_time;


public:
    std::vector<Shopper> shoppers;
    int service_time;
    bool in_service;
    bool end = false;
    std::vector<double> working_time;

    Shopping_mall(int ServicetTime);
    Shopping_mall();
    void Push_back_working_time(double work);
    void Push_back_waiting_time(double wait);
    void start_of_service();
    bool Check_empty_shoppers();
    void Pop_front_shoppers();
    void Push_back_shopper(Shopper shopper);
    bool Check_empty_waiting_time();
    std::vector<double> Return_waiting_time();
    void End();
    void shopper_add_times
    (std::chrono::high_resolution_clock::time_point point2, int number);
    void generation();
    std::vector<double> duration_work_shopper();
};

//void service(Cashier& cash); 
void servicing(Shopping_mall& mall);


//ÏÎÏÐÀÂÈÒÜ
//int CashChoose(Cashier casses[], int cash_count, int max_queue)
int Cash_Choose(Shopping_mall casses[], int cash_count, int max_queue);

double Factorial_Recursive(int n);

double Calculate_Average(std::vector<double> array);

double Degree(double num, int Degree);
