#pragma once

#include <functional>
#include <chrono>

using std::chrono::milliseconds;
using Action = std::function<void(void)>;

class TimedCaller
{
private:
	bool _shouldWork;

public:
	void Call(Action action, milliseconds duration, milliseconds timeout);
};
