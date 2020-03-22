#pragma once
#include <functional> //Contains std::function for passing in any type of function as parameters, including lambda expressions

//The thread pool used for executing code asychronously
namespace ThreadPool
{
	//Start the thread pool
	void Start();

	//Adds a new function to the thread pool queue to be executed
	void AddToQueue(std::function<void()>&& func);

	void Stop();
}