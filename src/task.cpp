// Copyright 2024 mpv-enjoyer
#include "task.h"

Task::Task(float time, float queue_begin_wait_time)
    : time(time), begin_begin(queue_begin_wait_time) {
    _cashbox = -1;
    _begin = -1;
}

Task::Task(const Task &task)
    : time(task.time), begin_begin(task.begin_begin) {
    _cashbox = task.cashbox;
    _begin = task.begin;
}

void Task::assign(float begin, int cashbox) {
    this->_begin = begin;
    this->_cashbox = cashbox;
}

bool Task::is_assigned() const {
    return begin != -1.0f;
}

bool Task::is_cashbox_available(float current_time) const {
    return begin + time <= current_time;
}

int Task::find_free(int cashbox_count,
                    float current_time,
                    const std::vector<Task> &tasks) {
    std::vector<bool> current_cashboxes(cashbox_count, true);
    for (int i = 0; i < tasks.size(); i++) {
        if (!tasks[i].is_assigned()) continue;
        if (tasks[i].is_cashbox_available(current_time)) continue;
        int index = tasks[i].cashbox;
        current_cashboxes[index] = false;
    }
    for (int i = 0; i < cashbox_count; i++) {
        if (current_cashboxes[i]) {
            return i;
        }
    }
    return -1;
}
