#pragma once

#include "AnimatedEntity.h"
#include <future>
#include <list>

struct PathResult
{
public:
	std::list<sf::Vector2f> Path{};
	bool Success = false;

	PathResult();
	PathResult(const std::list<sf::Vector2f>& path, bool success);
	PathResult(std::list<sf::Vector2f>&& path, bool success);
};

struct PathAsyncResult
{
	bool done = false;
	PathResult result;
};





class Enemy : public AnimatedEntity, public ObjectManager<Enemy>
{
	//const WorldMap& map;
	//int Strength = 1;

protected:
	PathResult GeneratePathToPlayer();
	std::shared_ptr<PathAsyncResult> GeneratePathToPlayerAsync();

public:

	static const std::vector<Enemy*>& GetAllEnemies();
	static std::recursive_mutex& GetEnemyListMutex();

	//int GetStrength() const;

	Enemy(const WorldMap& map,bool enableCollision = true);

	// Inherited via AnimatedEntity
	virtual void Update(sf::Time dt) override = 0;
};

