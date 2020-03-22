#pragma once
#include "Enemy.h" //Contains the Enemy class
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources
#include "Common.h" //Contains many common game functions and variables

//The enemy skeleton that tries to chase down the player in the game
class SkeletonEnemy : public Enemy
{
	int health = 3; //The health of the skeleton

	static ResourceTexture skeletonSheet; //The resource texture of the skeleton texture sheet

	//Stores the result of GeneratePathToPlayerAsync();
	std::shared_ptr<PathAsyncResult> AsyncResult = nullptr;

	float updateTimer = 0.0f; //Stores how often the enemy should update it's path to the player
	//How fast the skeleton should move around the map
	float movementSpeed = static_cast<float>(Common::RandomNumber(200, 2000)) * (80.0f / 2000.0f);

	std::list<sf::Vector2f> pathToPlayer; //Stores the path to the player

	float stunTimer = 0.0f; //Stores how long the enemy should be stunned for
	bool stunned = false; //Whether the enemy is stunned or not
	sf::Vector2f stunDirection; //The direction the enemy will travel when stunned

public:

	//Constructs a new skeleton enemy
	SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint);

	//The update loop for the skeleton
	virtual void Update(sf::Time dt) override;

	//Whether the enemy is stunned or not
	bool IsStunned() const;

	//Returns how much health the enemy has
	int GetHealth() const;

	//Whether the enemy is alive or not
	bool IsAlive() const;
};

