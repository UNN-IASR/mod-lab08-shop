#include <iostream>
#include "task.h"

int main() {
    setlocale(LC_ALL, "Rus");
    int numberCass = 3; //количество касс
    double intensityOfCustomerFlow = 0.5; //интенсивность потока покупателей (от 0 до 1)
    double speedProcessingCheckout = 0.1;  //скорость обработки товара на кассе (кол-во товара в милисекунду)
    int averageOfItemInCart = 10;  // среднее количество товаров в тележке покупателя
    int maxQueueLength = 5;
    Shop shop(numberCass, intensityOfCustomerFlow,
        speedProcessingCheckout, averageOfItemInCart, maxQueueLength);
    shop.Start();
    shop.Stata();
}
