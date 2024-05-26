#include "TimedCaller.h"
#include <thread>

void TimedCaller::Call(Action action, milliseconds duration, milliseconds timeout)
{
	_shouldWork = true;

	std::thread thread([&] {
		while (_shouldWork)
		{
			action();
			std::this_thread::sleep_for(timeout);
		}

		return;
	});

	std::this_thread::sleep_for(duration);
	_shouldWork = false;
	thread.join();
}
