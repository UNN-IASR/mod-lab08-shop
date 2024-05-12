// Copyright 2024 mpv-enjoyer
#pragma once
#include <functional>
#include <vector>

class Task {
    int _cashbox;
    float _begin;
 public:
    const float time;
    const float begin_begin;
    const int& cashbox = std::ref(_cashbox);
    const float& begin = std::ref(_begin);
    Task(float time, float queue_begin_wait_time);
    Task(const Task& task);
    void assign(float begin, int cashbox);
    bool is_assigned() const;
    bool is_cashbox_available(float current_time) const;
    static int find_free(int count, float time, const std::vector<Task> &tsks);
};
