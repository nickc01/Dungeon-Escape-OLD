#include "ThreadPool.h"
#include <thread>
#include <list>
#include <mutex>


using namespace std;

namespace
{
	bool running = false;
	thread poolThread;
	recursive_mutex poolMutex;

	list<function<void()>> threadPoolFunctions;

	void ThreadPoolMain();
}

void ThreadPool::Start()
{
	if (!running)
	{
		running = true;
		poolThread = thread(ThreadPoolMain);
	}
}

void ThreadPool::AddToQueue(std::function<void()>&& func)
{
	unique_lock<recursive_mutex> lock(poolMutex);

	threadPoolFunctions.emplace_back(std::move(func));
}

void ThreadPool::Stop()
{
	if (running)
	{
		running = false;
		poolThread.join();
	}
}



namespace
{
	void ThreadPoolMain()
	{
		while (true)
		{
			if (running == false)
			{
				break;
			}

			function<void()> currentFunction;

			{
				if (threadPoolFunctions.size() == 0)
				{
					continue;
				}
				unique_lock<recursive_mutex> lock(poolMutex);
				currentFunction = std::move(threadPoolFunctions.front());
				threadPoolFunctions.pop_front();
			}
			currentFunction();
		}
	}
}
