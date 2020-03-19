#pragma once
#include <functional>

namespace ThreadPool
{
	void Start();

	void AddToQueue(std::function<void()>&& func);

	void Stop();
}