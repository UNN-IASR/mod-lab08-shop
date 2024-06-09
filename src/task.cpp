#include "../include/task.h"

int randomInInterval(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(min, max);
    return dis(gen);
};

Shop::Shop(int registers, int intensity, int speed, int maxQueue) {
    cashRegistersNumber = registers;
    customerIntensity = intensity;
    processingSpeed = speed;
    queueLengthMax = maxQueue;
    customersServed = 0;
    customersServed = 0;
    queueLengthTotal = 0;
    customerStayTimeTotal = 0;
    checkoutTimeTotal = 0;
    idleTimeTotal = 0;
    isFinished = false;
    checkoutTimes.resize(registers, 0);
    timeTotal = 0;
};

void Shop::simulate(int simulationTime, double itemsAvg) {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> cashRegisterThreads;
    for (int i = 0; i < cashRegistersNumber; i++) {
        cashRegisterThreads.emplace_back(&Shop::cashierProcess, this);
    }
    std::thread customerThread(&Shop::customerProcess, this, itemsAvg);
    auto seconds = std::chrono::duration<double>(simulationTime);
    std::this_thread::sleep_for(seconds);
    isFinished = true;

    auto end = std::chrono::high_resolution_clock::now();
    timeTotal = (double)(end - start).count() / 1000000000;
    idleTimeTotal = timeTotal - checkoutTimeTotal / cashRegistersNumber;
    for (auto& thread : cashRegisterThreads) {
        thread.join();
    }
    customerThread.join();
};

void Shop::cashierProcess() {
    auto start = std::chrono::high_resolution_clock::now();

    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        auto current = std::chrono::high_resolution_clock::now();

        if (!customerQueue.empty()) {
            Customer customer = customerQueue.front();
            customerQueue.pop();

            auto operating = (double)customer.itemsCount / processingSpeed;
            auto checkoutTime = std::chrono::duration<double>(operating);

            lock.unlock();

            std::this_thread::sleep_for(checkoutTime);

            lock.lock();
            checkoutTimeTotal += operating;
            customerStayTimeTotal += (getCurrentTime() - customer.arrivalTime) + operating;
            customersServed++;
        }
        if (isFinished) {
            break;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
};

void Shop::customerProcess(double itemsAvg) {
    auto start = std::chrono::high_resolution_clock::now();

    while (!isFinished) {
        auto seconds = std::chrono::duration<double>((double)1 / customerIntensity);
        std::this_thread::sleep_for(seconds);

        Customer customer;
        customer.arrivalTime = getCurrentTime();
        customer.itemsCount = randomInInterval(0, 2 * itemsAvg);

        if (customerQueue.size() < queueLengthMax) {
            customerQueue.push(customer);
            queueLengthTotal += customerQueue.size();
        }
        else {
            customersServed++;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
}

int Shop::getCurrentTime() {
    return std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

Statistics Shop::printActualStatistics() {
    double queueLengthAvg = static_cast<double>(queueLengthTotal) / customersServed;
    double customerStayTimeAvg = static_cast<double>(customerStayTimeTotal) / customersServed;
    double checkoutTimeAvg = static_cast<double>(checkoutTimeTotal) / customersServed;
    double idleTimeAvg = static_cast<double>(timeTotal - checkoutTimeAvg);

    std::cout << "Served customers: " << customersServed << std::endl;
    std::cout << "Unserved customers: " << customersUnserved << std::endl;
    std::cout << "Average queue length: " << queueLengthAvg << std::endl;
    std::cout << "Average customer stay time: " << customerStayTimeAvg << std::endl;
    std::cout << "Average checkout time: " << checkoutTimeAvg << std::endl;
    std::cout << "Average idle time: " << idleTimeAvg << std::endl << std::endl;

    double probFail = static_cast<double>(customersServed) / (customersServed + customersServed);
    std::cout << "Actual failure probability: " << probFail << std::endl;
    double throughputRel = static_cast<double>(customersServed) / (customersServed + customersServed);
    std::cout << "Actual relative throughput : " << throughputRel << std::endl;
    double throughputAbs = static_cast<double>(customersServed) / timeTotal;
    std::cout << "Actual absolute throughput: " << throughputAbs << std::endl;

    Statistics statistics;
    statistics.customersServed = customersServed;
    statistics.customersUnserved = customersUnserved;
    statistics.probFail = probFail;
    statistics.throughputRel = throughputRel;
    statistics.throughputAbs = throughputAbs;
    return statistics;
}

Statistics Shop::printTheoreticStatistics(double intensity, double speed, int cashRegistersNumber,
    int max_queue_length, double itemsAvg) {
    double r = intensity * itemsAvg / speed;
    double P0 = getP0(cashRegistersNumber, r, max_queue_length);

    double probFail = getPN(cashRegistersNumber, r, P0, max_queue_length);
    std::cout << "Theoretic Probability of failure: " << probFail << std::endl;

    double throughputRel = 1 - probFail;
    std::cout << "Theoretic Relative throughput of the shop: " << throughputRel << std::endl;

    double throughputAbs = itemsAvg * intensity * throughputRel;
    std::cout << "Theoretic Absolute throughput of the shop: " << throughputAbs << std::endl;

    Statistics statistics;
    statistics.probFail = probFail;
    statistics.throughputRel = throughputRel;
    statistics.throughputAbs = throughputAbs;
    return statistics;
}

double getP0(int poolCount, double r, int maxLength) {
    double P0 = 0;
    for (int i = 0; i <= poolCount; i++) {
        P0 = P0 + pow(r, i) / factorial(i);
    }
    for (int i = 1; i <= maxLength; i++) {
        P0 = P0 + pow(r, poolCount + i) / (factorial(poolCount) * pow(poolCount, i));
    }
    return pow(P0, -1);
}

double getPN(int poolCount, double r, double P0, int maxLength) {
    return pow(r, poolCount + maxLength) / (factorial(poolCount) * pow(poolCount, maxLength)) * P0;
}

double getR(double lambda, double nu) {
    return lambda / nu;
}

double factorial(double n) {
    double factorial = 1;
    for (int i = 1; i <= n; i++) {
        factorial = factorial * i;
    }
    return factorial;
}