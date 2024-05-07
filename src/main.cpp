#include "task.h"

double avg_value(std::vector<double> values) {
  double sum = 0;
  double count = values.size();
  for (int j = 0; j < count; j++) {
    sum += values[j];
  }
  return sum/count;
}

double factorial(int n) {
  double num = 1;
  if(n >= 1) {
    for (int i = 1; i <= n; i++) {
    num *= i;
    }
  }
  return num;
}

double power(double num, int pow) {
  double res = num;
  if(pow >= 2) {
    for (int i = 2; i <= pow; i++) {
      res *= num;
    }
  }
  
  if (pow == 0) return 1;
  return res;
}

int main()
{
  setlocale(LC_ALL, "Rus");
  //input params
  const int cash_count = 5;
  const int lambda = 100;
  const int serv_time = 100; // 1/myu
  const int avg_products = 4;
  const int max_queue_length = 5;

  //statistic values
  int passed_clients = 0;
  int missed_clients = 0;
  double avg_queue_length = 0;
  double avg_overall_time = 0;
  double avg_cash_time = 0;
  double avg_onhold_time = 0;

  //to_calculate
  double Prej = 0;
  double Q = 0;
  double A = 0;

  //program execution
  Cashier casses[cash_count];
  for(int i = 0; i < cash_count; i++) {
    casses[i] = Cashier(serv_time);
    casses[i].spawn();
  }

  Client clients[100];
  std::vector<double> queues, times_at_cash;
  for(int j = 0; j < 100; j++) {
    clients[j] = Client(avg_products, j);
    int num = CashChoose(casses, cash_count, max_queue_length);
    if(num != -1) {
      casses[num].client_add(clients[j]);
      std::cout << "Клиент " << j+1 << 
      " встал в очередь кассы " << num+1 << std::endl;
      passed_clients++; //
    } else {
      missed_clients++; //
      std::cout << "Клиент " << j+1 <<
      " не смог встать в очередь и ушел " << std::endl;
    }

    std::vector<double> queues_in_iter; //avg vec len for iteration
    for (int i = 0; i < cash_count; i++) {
      queues_in_iter.push_back(casses[i].queue_size());
    }
    queues.push_back(avg_value(queues_in_iter)); //and add in common vector

    std::this_thread::sleep_for(std::chrono::milliseconds(1000/lambda));
  }

  std::this_thread::sleep_for(std::chrono::milliseconds(3000));

  for (int i = 0; i < cash_count; i++) {
    casses[i].stop();
    times_at_cash.push_back(avg_value(casses[i].durations_client()));
  }

  std::vector<double> work_time, hold_time;
  for (int i = 0; i < cash_count; i++) {  //avg of *cash_count* casses
    work_time.push_back(avg_value(casses[i].Work_Times()));
    hold_time.push_back(avg_value(casses[i].Hold_Times()));
  }

  //calculating
  avg_queue_length = avg_value(queues);
  avg_overall_time = avg_value(times_at_cash);
  avg_cash_time = avg_value(work_time);
  avg_onhold_time = avg_value(hold_time);

  Prej = (double)missed_clients / (missed_clients + passed_clients);
  Q = 1 - Prej;
  A = Q * lambda;
  
  std::cout << "Экспериментально вычисленные данные: " << std::endl;
  std::cout << "  Число обслуженных клиентов: " <<
  passed_clients << std::endl;
  std::cout << "  Число необслуженных клиентов: " <<
  missed_clients << std::endl;
  std::cout << "  Средняя длина очереди: " << avg_queue_length
  << std::endl;
  std::cout << "  Среднее время в очереди + на кассе: " <<
  avg_overall_time << std::endl;
  std::cout << "  Среднее время работы кассы: " <<
  avg_cash_time << std::endl;
  std::cout << "  Среднее время простоя кассы: " <<
  avg_onhold_time << std::endl;
  std::cout << "  Вероятность отказа: " << Prej << std::endl;
  std::cout << "  Относительная пропускная способность: "
  << Q << std::endl;
  std::cout << "  Абсолютная пропускная способность: "
  << A << std::endl;
  std::cout << std::endl;
   
  //theory calculations
  double myu = 1000 / serv_time;
  double ro = lambda / myu;
  double sum = 0;
  for (int i = 0; i <= cash_count + max_queue_length; i++) {
    if (i <= cash_count) {
      sum += power(ro, i) / factorial(i);
    } else {
      sum += power(ro, i) / (factorial(cash_count) *
      power(cash_count, i - cash_count));
    }
  }
  double P0 = (double)1 / sum;
  Prej = P0 * power(ro, cash_count + max_queue_length) /
  (power(cash_count, max_queue_length) * factorial(cash_count));
  Q = 1 - Prej;
  A = lambda * Q;

  std::cout << "Теоретически вычисленные данные: " << std::endl;
  std::cout << "  Интенсивность потока клиентов: " << lambda << std::endl;
  std::cout << "  Интенсивность обслуживания: " << myu << std::endl;
  std::cout << "  Интенсивность нагрузки канала: " << ro << std::endl;
  std::cout << "  Вероятность отказа: " << Prej << std::endl;
  std::cout << "  Относительная пропускная способность: " << Q
  << std::endl;
  std::cout << "  Абсолютная пропускная способность: " << A << std::endl;
  getchar();
}

