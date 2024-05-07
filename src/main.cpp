// Copyright 2024 Danil Volkov

#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include "../include/task.h"

int main() {
    // int numThreads = 1; // Количество касс
    // int maxQueueLength = 1; // Максимальная длина очереди на каждой кассе
    // int numCustomers = 3; // Общее количество покупателей
    // double intensityCustomers = 1.0; // Интенсивность потока покупателей в секунду
    // int averageItems = 5; // Среднее количество товаров в тележке покупателя
    // double speedProductCash = 2.0; // скорость обработки товара на кассе в секунду

    int numThreads = 3; // Количество касс
    int maxQueueLength = 5; // Максимальная длина очереди на каждой кассе
    int numCustomers = 20; // Общее количество покупателей
    double intensityCustomers = 5.0; // Интенсивность потока покупателей в секунду
    int averageItems = 10; // Среднее количество товаров в тележке покупателя
    double speedProductCash = 10.0; // скорость обработки товара на кассе в секунду

    setlocale(LC_ALL, "");

    Shop shop(numThreads, maxQueueLength, speedProductCash);
    Client client(&shop, averageItems);

    // Создаем потоки для каждой кассы
    std::vector<std::thread> serverThreads;
    for (int i = 0; i < numThreads; ++i) {
        serverThreads.push_back(std::thread(&Shop::answer, &shop));
    }

    // Генерируем покупателей
    for (int id = 1; id <= numCustomers; id++) {
        client.send(id);
        std::this_thread::sleep_for(std::chrono::milliseconds((int)(1000 / intensityCustomers))); // Пауза между приходом покупателей
    }

    // Уведомляем сервер о том, что все клиенты обработаны
    shop.setAllCustomersProcessed();

    // Дожидаемся завершения всех потоков касс
    for (auto& t : serverThreads) {
        t.join();
    }

    // Выводим статистику
    std::cout << "Всего клиентов: " << shop.getRequestCount() << std::endl;
    std::cout << "Обработано клиентов: " << shop.getProcessedCount() << std::endl;
    std::cout << "Отклонено клиентов: " << shop.getRejectedCount() << std::endl;
    std::cout << "Средняя длина очереди: " << shop.getAverageQueueLength() << std::endl;
    std::cout << "Среднее время нахождения покупателя в очереди: " << shop.getAverageQueueTime() << " сек." << std::endl;
    std::cout << "Среднее время нахождения покупателя на кассе: " << shop.getAverageCashierTime() << " сек." << std::endl;
    std::cout << "Среднее время работы кассы: " << shop.getAverageCashierTime() << " сек." << std::endl;
    std::cout << "Среднее время простоя кассы: " << shop.getAverageCashierTime() - shop.getAverageQueueTime() << " сек." << std::endl;

    double probablityIdle = (double)shop.getRejectedCount() / shop.getRequestCount() * 100;
    double realtiveCapacity = (double)shop.getProcessedCount() / (shop.getProcessedCount() + shop.getRejectedCount()) * 100;
    double absoluteCapacity = intensityCustomers * realtiveCapacity;
    std::cout << "Вероятность отказа магазина: " << probablityIdle << "%" << std::endl;
    std::cout << "Относительная пропускная способность магазина: " << realtiveCapacity << "%" << std::endl;
    std::cout << "Абсолютная пропускная способность магазина: " << absoluteCapacity << "%" << std::endl;

    std::cout << std::endl << "Статистика из формул: " << std::endl;

    double channelLoadIntensity = intensityCustomers / speedProductCash; // интенсивность нагрузки канала
    probablityIdle = (std::pow(channelLoadIntensity, numThreads) / factorial(numThreads))
        * (1 - std::pow(channelLoadIntensity / numThreads, maxQueueLength))
        / (1 - channelLoadIntensity / numThreads) * 100; // вероятность отказа
    realtiveCapacity = 100 - probablityIdle;
    absoluteCapacity = intensityCustomers * realtiveCapacity;

    std::cout << "Вероятность отказа магазина: " << probablityIdle << "%" << std::endl;
    std::cout << "Относительная пропускная способность магазина: " << realtiveCapacity << "%" << std::endl;
    std::cout << "Абсолютная пропускная способность магазина: " << absoluteCapacity << "%" << std::endl;

    std::ofstream outputFile("report.txt");

    if (outputFile.is_open()) {
        outputFile << "Статистика из программы: " << std::endl;
        outputFile << "Всего клиентов: " << shop.getRequestCount() << std::endl;
        outputFile << "Обработано клиентов: " << shop.getProcessedCount() << std::endl;
        outputFile << "Отклонено клиентов: " << shop.getRejectedCount() << std::endl;
        outputFile << "Средняя длина очереди: " << shop.getAverageQueueLength() << std::endl;
        outputFile << "Среднее время нахождения покупателя в очереди: " << shop.getAverageQueueTime() << " сек." << std::endl;
        outputFile << "Среднее время нахождения покупателя на кассе: " << shop.getAverageCashierTime() << " сек." << std::endl;
        outputFile << "Среднее время работы кассы: " << shop.getAverageCashierTime() << " сек." << std::endl;
        outputFile << "Среднее время простоя кассы: " << shop.getAverageCashierTime() - shop.getAverageQueueTime() << " сек." << std::endl;
        outputFile << "Вероятность отказа магазина: " << probablityIdle << "%" << std::endl;
        outputFile << "Относительная пропускная способность магазина: " << realtiveCapacity << "%" << std::endl;
        outputFile << "Абсолютная пропускная способность магазина: " << absoluteCapacity << "%" << std::endl;
        outputFile << std::endl << "Статистика из формул: " << std::endl;
        outputFile << "Вероятность отказа магазина: " << probablityIdle << "%" << std::endl;
        outputFile << "Относительная пропускная способность магазина: " << realtiveCapacity << "%" << std::endl;
        outputFile << "Абсолютная пропускная способность магазина: " << absoluteCapacity << "%" << std::endl;
        
        outputFile << std::endl << "Вывод: Статистические данные отличаются от теоретических,"
            << " но изменение входных данных сохраняет тенденцию к увеличению или уменьшению значений."
            << " Такие различия могут быть вызваны упрощениями модели или недостаточным учетом факторов."
            << std::endl;

        outputFile.close();
    }
    else {
        std::cerr << "Не удалось открыть файл для записи." << std::endl;
    }

    return 0;
}
