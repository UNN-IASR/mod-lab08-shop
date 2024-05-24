#include <iostream>
#include <cmath>
#include "task.cpp"

unsigned int factorial(unsigned int n)
{
    int res = 1, i;
    for (i = 2; i <= n; i++)
        res *= i;
    return res;
}

int main()  {
    int num_of_cash_reg = 4;
    int buyers_intensity = 6;
    int cash_reg_speed = 20;
    int avg_capacity_cart = 10;
    int max_queue_size = 5;
    Shop_working shop_working(num_of_cash_reg,
                              buyers_intensity,
                              cash_reg_speed,
                              avg_capacity_cart,
                              max_queue_size);
    shop_working.Start_sim(100);
    
    double lambda = buyers_intensity;
    double mu = 1.0*cash_reg_speed/avg_capacity_cart;
    double p = lambda/mu;
    double sum_P0 = 0;
    for (int i = 0; i < num_of_cash_reg + 1; i++) {
        sum_P0+=std::pow(p, i)*1.0/factorial(i);
    }
    for (int i = num_of_cash_reg + 1; i < num_of_cash_reg + max_queue_size + 1; i++) {
        sum_P0+=std::pow(p, i)*1.0/(factorial(num_of_cash_reg)*std::pow(num_of_cash_reg, i - num_of_cash_reg));
    }
    double P0 = 1.0/sum_P0;
    double Prej = P0*std::pow(p, num_of_cash_reg+max_queue_size)*1.0/(std::pow(num_of_cash_reg, max_queue_size)*factorial(num_of_cash_reg));
    double Lq = P0*(std::pow(p, num_of_cash_reg + 1)*1.0/(num_of_cash_reg*factorial(num_of_cash_reg)))*(1 - std::pow(p*1.0/num_of_cash_reg, max_queue_size)*(1 + max_queue_size*(1 - p*1.0/num_of_cash_reg)))/(std::pow(1 - p/num_of_cash_reg, 2));
    double Q = 1 - Prej;
    double A = lambda*Q;
    double Lsrv = A/mu;
    double t = Lq/lambda + Q/mu;
    double tq = Lq/lambda;
    std::cout << "Theoretically:" << std::endl;
    std::cout << "Probability of failure: " << Prej << std::endl;
    std::cout << "Relative throughput:    " << Q << std::endl;
    std::cout << "Absolute bandwidth:     " << A << std::endl;
    std::cout << "MAIN FINISH!!!😐" << std::endl;
    return 0; 
}