#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <queue>
#include <mutex>
#include <map>
#include <Windows.h>

class Client {
    private:
        int number;
    public:
        Client(int num, std::chrono::system_clock::time_point CurrentTime, int Count);
        int CountProducts;
        std::chrono::system_clock::time_point TimeInQueue;
};

class Tsum {
    private:
        bool isBusy = true;
        int CountCassa;
        double IntensityCustomers;
        int ProcessingSpeed;
        int AverageNumberOfProducts;
        int MaxLengthQueue;
        std::queue<Client> queue;
        std::mutex mutex;
        std::vector<std::thread> NumberThreads;
    public:
        Tsum(int Count_Cassa, double Intensity_Customers, int Processing_Speed, int Average_Number_Of_Products, int Max_Length_Queue);
        void Execute(int NumberCustomers);
        void OpenCassa();
        void Processing(Client client, int TimeClient);
        int ServicedCustomers = 0;
        int UnServicedCustomers = 0;
        int AverageBusyCassa = 0;
        int AverageQueueLength = 0;
        int TimeQueue = 0;
        int TimeCassa = 0;
};

class Teoretic {
     private:
        bool isBusy;
        int CountCassa;
        double IntensityCustomers;
        int ProcessingSpeed;
        int AverageNumberOfProducts;
        int MaxLengthQueue;
        std::chrono::system_clock::time_point TimePoint;
        std::queue<int> queue;
        std::mutex mtx;
    public:
        Teoretic(int Count_Cassa, double Intensity_Customers, int Processing_Speed, int Average_Number_Of_Products, int Max_Length_Queue);
        void StartCalculation(); 
        int Factorial(int n);
};
