#include "GameManager.h"
#include "WorldMap.h"
#include "Common.h"
#include "Player.h"
#include <thread>
#include "SkeletonEnemy.h"
#include "ThreadPool.h"
#include "HeartDisplay.h"

using namespace std;
using namespace sf;

atomic<bool> GameManager::EndingGame = false;

void GameManager::StartGame()
{

	ThreadPool::Start();
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

	std::vector<std::shared_ptr<SkeletonEnemy>> skeletons{};

	//shared_ptr<SkeletonEnemy> testSkeleton = nullptr;

	Player player(map, static_cast<Vector2f>(map.GetSpawnPoint()));

	HeartDisplay display = HeartDisplay(player);

	{
		unique_lock<recursive_mutex> lock(ObjectManager<Renderable>::GetMutex());

		for (auto& enemyPoint : map.GetEnemySpawnPoints())
		{
			skeletons.push_back(make_shared<SkeletonEnemy>(map, enemyPoint));
			//break;
			//skeletons.emplace_back(map, enemyPoint);
			/*if (skeletons.size() == 2)
			{
				break;
			}*/
			//testSkeleton = make_shared<SkeletonEnemy>(map,enemyPoint);
		}

	}

	while (true)
	{
		if (EndingGame)
		{
			break;
		}
	}

	ThreadPool::Stop();
}
