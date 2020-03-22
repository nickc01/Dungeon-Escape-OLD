#pragma once

#include "AnimatedEntity.h" //Contains the AnimatedEntity class for entities that are animated
#include <list> //Contains std::list for storing items in a linked list

//Represents the results of calling the GeneratePathToPlayer() function
struct PathResult
{
public:
	std::list<sf::Vector2f> Path{}; //The path the enemy will take to get to the player
	bool Success = false; //Whether the algorithm was able to find the player or not

	//Constructs an empty results object
	PathResult();

	//Constructs a new results object with the specified parameters
	PathResult(const std::list<sf::Vector2f>& path, bool success);
	//Constructs a new results object with the specified parameters
	PathResult(std::list<sf::Vector2f>&& path, bool success);
};

//Represents the results of calling the GeneratePathToPlayerAsync() function
struct PathAsyncResult
{
	//Whether the results are ready or not
	bool done = false;
	//The results
	PathResult result;
};





class Enemy : public AnimatedEntity, public ObjectManager<Enemy>
{

protected:
	//Uses a pathfinding algorithm to find a path to the player
	PathResult GeneratePathToPlayer();

	//Generates a path asynchronously. You will know when it's done when the "done" field is set to true
	std::shared_ptr<PathAsyncResult> GeneratePathToPlayerAsync();

public:

	//Gets all currently spawned enemies
	static const std::vector<Enemy*>& GetAllEnemies();

	//Gets the mutex that controls the enemy list. Used to prevent data races
	static std::recursive_mutex& GetEnemyListMutex();

	//Constructs a new enemy
	Enemy(const WorldMap& map,bool enableCollision = true);

	//The update function that is called each frame
	virtual void Update(sf::Time dt) override = 0;
};

