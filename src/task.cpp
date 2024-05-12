#include "../include/task.h"

Tsum::Tsum(int Count_Cassa, double Intensity_Customers, int Processing_Speed, int Average_Number_Of_Products, int Max_Length_Queue) {
    CountCassa = Count_Cassa;
    IntensityCustomers = Intensity_Customers;
    ProcessingSpeed = Processing_Speed;
    AverageNumberOfProducts = Average_Number_Of_Products;
    MaxLengthQueue = Max_Length_Queue;
}

Teoretic::Teoretic(int Count_Cassa, double Intensity_Customers, int Processing_Speed, int Average_Number_Of_Products, int Max_Length_Queue) {
    CountCassa = Count_Cassa;
    IntensityCustomers = Intensity_Customers;
    ProcessingSpeed = Processing_Speed;
    AverageNumberOfProducts = Average_Number_Of_Products;
    MaxLengthQueue = Max_Length_Queue;
}

Client::Client(int num, std::chrono::system_clock::time_point CurrentTime, int Count) {
    number = num;
    TimeInQueue = CurrentTime;
    CountProducts = Count;
}

void Teoretic::StartCalculation() {
    double nu = 1000.0*AverageNumberOfProducts/ProcessingSpeed;
    double ro = IntensityCustomers/nu;
    double intp = 0;
    for (int i=0; i<=CountCassa; i++) {
        intp+=(double)pow(ro,i)/Factorial(i); 
    }
    for (int i=1; i<=MaxLengthQueue; i++) {
        intp+=(double)pow(ro,CountCassa + i)/Factorial(CountCassa)*(pow(CountCassa,i));
    }
    double p0 = (double)1/intp;
    double pn = p0*(pow(ro, CountCassa) / Factorial(CountCassa))*((1-pow(ro/CountCassa, MaxLengthQueue))/(1-ro/CountCassa));
    double pre = p0*pow(ro, CountCassa+MaxLengthQueue)/(pow(CountCassa, MaxLengthQueue)*Factorial(CountCassa));
    double Q = 1-pre;
    double A = IntensityCustomers*Q;

    std::cout<<"Теория"<<std::endl;
    std::cout<<"Вероятность отказа:"<< pre << std::endl;
    std::cout<<"Относительная пропускная способность:"<< Q << std::endl;
    std::cout<<"Абсолютная пропускная способность:"<< A << std::endl;
}

int Teoretic::Factorial(int n) {
    int res = (n == 0) ? 1 : n * Factorial(n - 1);
    return res;
}

void Tsum::Execute(int NumberCustomers) {
    for (int i=0; i < CountCassa; i++) {
        NumberThreads.push_back(std::thread(&Tsum::OpenCassa, this));
    }
    std::chrono::system_clock::time_point CurrentTime;
    for(int i = 0; i < NumberCustomers; i++) {
        CurrentTime = std::chrono::system_clock::now();
        int Count = rand() % (int)(AverageNumberOfProducts*2-1) + 1;
        Client client (i, CurrentTime, Count);
        if (queue.size() < MaxLengthQueue) {
            ServicedCustomers++;
            queue.push(client);
        }
        else {
            UnServicedCustomers++;
        }
        AverageQueueLength += queue.size();
        auto RandomDelay =  std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(1 / IntensityCustomers));
        std::this_thread::sleep_for(std::chrono::milliseconds(RandomDelay));
    }
    isBusy = false;
    for (int i=0; i < CountCassa; i++) {
        NumberThreads[i].join();
    }
    auto Finish = std::chrono::system_clock::now();
    TimeCassa = std::chrono::duration_cast<std::chrono::milliseconds>(Finish - CurrentTime).count();
}

void Tsum::OpenCassa() {
    while (isBusy || queue.size() != 0) {
        //std::cout<<"1";
        int TimeClient;
        if (queue.size() == 0) continue;
        {std::lock_guard<std::mutex> lock(mutex);
        Client client = queue.front();
        queue.pop();
        TimeClient = client.CountProducts*ProcessingSpeed;
        Processing(client, TimeClient);}
        std::this_thread::sleep_for(std::chrono::milliseconds(TimeClient));
    }
}

void Tsum::Processing(Client client, int TimeClient) {
    std::chrono::system_clock::time_point CurrentTime = std::chrono::system_clock::now();
    TimeQueue += std::chrono::duration_cast<std::chrono::milliseconds>(CurrentTime - client.TimeInQueue).count();
    AverageBusyCassa += TimeClient;
}
