#pragma once
#include "Enemy.h"
#include "ResourceTexture.h"
#include "Common.h"


class SkeletonEnemy : public Enemy
{
	static ResourceTexture skeletonSheet;

	std::shared_ptr<PathAsyncResult> AsyncResult = nullptr;

	float updateTimer = 0.0f;
	float movementSpeed = static_cast<float>(RandomNumber(200, 2000)) * (60.0f / 2000.0f);

	//int pathIndex = 0;
	std::list<sf::Vector2f> pathToPlayer;

public:

	SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint);

	virtual void Update(sf::Time dt) override;
};

