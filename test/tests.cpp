// Copyright 2021 GHA Test Team
#include <gtest/gtest.h>
#include "../include/task.h"

TEST(test1, no_rejected_clients) {
    market m { 2, 2, 100, 5, 2 };
    m.run(20);
    stats s = m.get_stats();
    EXPECT_EQ(s.rejected_clients, 0);
}

TEST(test2, avg_queue_len_is_1) {
    market m { 1, 5, 100, 5, 1 };
    m.run(20);
    stats s = m.get_stats();
    EXPECT_EQ(s.avg_queue_len, 0);
}

TEST(test3, no_queue_no_work) {
    market m { 1, 5, 100, 5, 0 };
    m.run(20);
    stats s = m.get_stats();
    EXPECT_EQ(s.accepted_clients, 0);
    EXPECT_EQ(s.avg_cash_worktime, 0)
}

TEST(test4, correct_worktime) {
    market m { 1, 5, 100, 5, 1 };
    time_point start = std::chrono::system_clock::now();
    m.run(20);
    time_point end = std::chrono::system_clock::now();
    stats s = m.get_stats();
    EXPECT_EQ(
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(),
        s.market_worktime
    );
}

TEST(test5, correct_poiss_times) {
    int lambda = 5;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::poisson_distribution<int> p_flow(lambda);
    std::vector<int> times;
    while (times.size() < 1000) {
        int r = p_flow(mt);
        int t = 1000 / (r == 0 ? 1 : r);
        for (int i = 0; i < r; i++)
            times.push_back(t);
    }
    EXPECT_EQ(
        abs(lambda - 1000.0 / (std::accumulate(times.begin(), times.end(), 0.0) / times.size())) < 0.1,
        true
    )
}