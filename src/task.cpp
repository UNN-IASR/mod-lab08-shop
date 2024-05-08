#include "../include/task.h"

Store::Store(int _cashDeskCount, double _customerIntensity, int _productProcessingSpeed, int _avgProductCount, int _maxQueueLength, int _countCustomer) {
    cashDeskCount = _cashDeskCount;
    customerIntensity = _customerIntensity;
    productProcessingSpeed = _productProcessingSpeed;
    avgProductCount = _avgProductCount;
    maxQueueLength = _maxQueueLength;
    countCustomer = _countCustomer;
    isOpen = true;
}

void Store::serveCustomer() {
    while (isOpen) {
        int customer;
        int itemsInCart;
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (!q.empty()) {
                customer = q.front();
                q.pop();
                itemsInCart = itemsInCartMap[customer];
                itemsInCartMap.erase(customer);
            } else {
                continue;
            }
            //std::cout << "Cashier " << std::this_thread::get_id() << " is serving customer " << customer << std::endl;
        }
        busyCashierTime += productProcessingSpeed * itemsInCart;
        std::this_thread::sleep_for(std::chrono::milliseconds(productProcessingSpeed * itemsInCart));
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::chrono::system_clock::time_point storeOut = std::chrono::system_clock::now();
            auto durationSinceEpoch = storeOut.time_since_epoch();
            timeInQueue += std::chrono::duration_cast<std::chrono::milliseconds>(storeOut - start).count();
        }
        //std::cout << "Cashier " << std::this_thread::get_id() << " finished serving customer " << customer << std::endl;
    }
}

int Store::getRandomArrivalTime() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<int> dis(customerIntensity - 1);
    return static_cast<int>(1000.0 / dis(gen));
}

int Store::getRandomProductCount() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::poisson_distribution<> dis(avgProductCount);
    int count = dis(gen);
    if (count == 0) return 1;
    else return count;
}

void Store::generateCustomer() {
    int counter = 1;
    while (true) {
        std::this_thread::sleep_for(std::chrono::milliseconds(getRandomArrivalTime()));
        int itemsInCart = getRandomProductCount();
        {
            std::lock_guard<std::mutex> lock(mtx);
            if (q.size() < maxQueueLength) {
                q.push(counter);
                itemsInCartMap[counter] = itemsInCart;
                acceptedCustomer++;
                std::chrono::system_clock::time_point queueIn = std::chrono::system_clock::now();
                auto durationSinceEpoch = queueIn.time_since_epoch();
                timeInQueue -= std::chrono::duration_cast<std::chrono::milliseconds>(queueIn - start).count();
                //std::cout << "Customer " << counter << " with " << itemsInCart << " items arrived." << std::endl;
            } else {
                rejectedCustomer++;
                //std::cout << "Customer " << counter << " left because the queue is full." << std::endl;
            }
            sumQueueLength += q.size();
        }
        counter++;
        if(counter == countCustomer + 1) {
            isOpen = false;
            break;
        }
    }
}

void Store::simulate() {
    std::vector<std::thread> cashDeskThreads;
    start = std::chrono::system_clock::now();
    for(int i = 0; i < cashDeskCount; i++) {
        std::thread t(&Store::serveCustomer, this);
        cashDeskThreads.push_back(std::move(t));
    }

    std::thread customerThread(&Store::generateCustomer, this);

    for (auto& thread : cashDeskThreads) {
        thread.join();
    }
    customerThread.join();
    std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
    storeTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
}