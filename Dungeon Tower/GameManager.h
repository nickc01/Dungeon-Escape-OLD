#pragma once

#include <atomic>

namespace GameManager
{
	extern std::atomic<bool> EndingGame;

	void StartGame();
}

