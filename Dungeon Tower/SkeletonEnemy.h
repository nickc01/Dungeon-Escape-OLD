#pragma once
#include "Enemy.h"
#include "ResourceTexture.h"
#include "Common.h"


class SkeletonEnemy : public Enemy
{
	int health = 3;

	static ResourceTexture skeletonSheet;

	std::shared_ptr<PathAsyncResult> AsyncResult = nullptr;

	float updateTimer = 0.0f;
	float movementSpeed = static_cast<float>(RandomNumber(200, 2000)) * (80.0f / 2000.0f);

	//int pathIndex = 0;
	std::list<sf::Vector2f> pathToPlayer;

	float stunTimer = 0.0f;
	bool stunned = false;
	sf::Vector2f stunDirection;

public:

	SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint);

	virtual void Update(sf::Time dt) override;

	bool IsStunned() const;

	int GetHealth() const;
	bool IsAlive() const;
};

