#include "GameManager.h"
#include "WorldMap.h"
#include "Common.h"
#include "Player.h"
#include <thread>

using namespace std;
using namespace sf;

atomic<bool> GameManager::EndingGame = false;

void GameManager::StartGame()
{
	WorldMap map{};

	map.GenerateAsync(20);

	while (true)
	{
		if (map.GetProgress() >= 1.0f)
		{
			break;
		}
	}

	CenterCamera(Vector2f(map.GetWidth() * 8.0f,map.GetHeight() * 8.0f ));

	map.EnableRendering();

	Player player = Player(map, static_cast<Vector2f>(map.GetSpawnPoint()));

	while (true)
	{
		if (EndingGame)
		{
			break;
		}
	}
	/*while (!EndingGame)
	{

	}*/
}
