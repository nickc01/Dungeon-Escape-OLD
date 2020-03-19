#include "SkeletonEnemy.h"
#include "Common.h"
#include "Math.h"

using namespace std;
using namespace sf;

namespace
{
	constexpr float RefreshTime = 5.0f;
}

ResourceTexture SkeletonEnemy::skeletonSheet{SKELETON_SHEET};

SkeletonEnemy::SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint) :
	Enemy(map,RandomNumber(1 + map.GetLevel(),3 + (map.GetLevel() * 2)),false,false)
{
	try
	{

		EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8));
		EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8));
		EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8));

		EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8));
		EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8));
		EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8));

		EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(0, 0, 6, 8));
		EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(7, 0, 6, 8));
		EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(14, 0, 6, 8));

		EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8), Vector2f(-1.0f, 1.0f));
		EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8), Vector2f(-1.0f, 1.0f));
		EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8), Vector2f(-1.0f, 1.0f));

		SetSpriteGroup("DOWN");

		auto rect = GetSprite()->getTextureRect();

		SetRenderLayer(8);

		GetSprite()->setPosition(spawnPoint);
		GetSprite()->setOrigin(rect.width / 2.0f, rect.height / 2.0f);


		UpdateReceiver::SetActive(true);

		Renderable::SetActive(true);
	}
	catch (...)
	{

		throw;
	}
}

void SkeletonEnemy::Update(sf::Time dt)
{

	if (AsyncResult != nullptr && AsyncResult->done)
	{
		pathToPlayer = AsyncResult->result.Path;
		//pathIndex = 0;
		AsyncResult = nullptr;

		if (pathToPlayer.size() > 0)
		{
			pathToPlayer.pop_front();
			//pathToPlayer.pop
		}
	}


	float time = dt.asSeconds();

	updateTimer += time;
	if (updateTimer >= RefreshTime || (pathToPlayer.size() == 0 && updateTimer > 1.0f))
	{
		updateTimer -= RefreshTime;

		if (AsyncResult == nullptr)
		{
			AsyncResult = GeneratePathToPlayerAsync();
			//auto future = GeneratePathToPlayerAsync();
			//AsyncResult = std::shared_ptr<std::future<PathResult>>(new std::future<PathResult>(std::move(future)));
			//AsyncResult = std::make_shared<std::future<PathResult>>(std::move(GeneratePathToPlayerAsync()));
		}

		//pathToPlayer = GeneratePathToPlayer().Path;
		//pathIndex = 0;
	}

	if (pathToPlayer.size() > 0)
	{
		Vector2f distanceToNextPoint = pathToPlayer.front() - GetSprite()->getPosition();

		Vector2f amountToMove = Math::NormalizeVector(distanceToNextPoint, movementSpeed * time);

		Move(amountToMove);

		UpdateAnimations(dt);

		if (Math::DistanceBetweenVectors(pathToPlayer.front(),GetSprite()->getPosition()) <= 1.0f)
		{
			pathToPlayer.pop_front();
		}
	}
}
