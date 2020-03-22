#pragma once
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources
#include <list> //Contains std::list for storing items in a linked list
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include "Direction.h" //Contains the Direction Enum for specifying the direction
#include <mutex> //Contains std::mutex and std::recursive_mutex for resource locking and to prevent data races //Contains std::mutex and std::recursive_mutex for mutual exclusion of resources and prevents data races
#include "Entity.h" //Contains the Entity class
#include "WorldMap.h" //Contains the WorldMap class

//The magic orb the player shoots to destroy enemies
class MagicOrb : public Entity
{
	static ResourceTexture orbTexture; //The texture resource for the magic orb
	static std::recursive_mutex orbsMutex; //The mutex of the list of spawned orbs
	static std::list<std::shared_ptr<MagicOrb>> SpawnedOrbs; //The list of spawned orbs


	float spawnInvincibilityTimer = 0.0f; //Stores the time left the magic orb is invincible to wall collisions
	float lifeTimeCounter = 0.0f; //Stores the time left the magic orb has to live
	const WorldMap& map; //The map the orb lives in
	sf::Sprite orbSprite; //The sprite of the magic orb
	sf::Vector2f direction; //The direction the orb is traveling in
	bool alive = true; //Whether the orb is alive or not

	//Constructs a new magic orb
	MagicOrb(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction);

	//The update function of the magic orb
	virtual void Update(sf::Time dt) override;

	//Renders the magic orb to the screen
	virtual void Render(sf::RenderWindow& window) override;

public:

	//Deleting these four functions prevents the object from being copied or moved
	MagicOrb(const MagicOrb& copy) = delete;
	MagicOrb(MagicOrb&& move) = delete;
	MagicOrb& operator=(const MagicOrb& copy) = delete;
	MagicOrb& operator=(MagicOrb&& move) = delete;

	//Gets the mutex for the list of spawned orbs
	static std::recursive_mutex& GetOrbListMutex();

	//Gets the list of all the spawned orbs
	static const std::list<std::shared_ptr<MagicOrb>>& GetAllOrbs();

	//Fires a new magic orb
	static std::shared_ptr<MagicOrb> Fire(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction);
	//Fires a new magic orb
	static std::shared_ptr<MagicOrb> Fire(const WorldMap& map, sf::Vector2f spawnPoint, Direction direction);

	//Destroys the orb
	void Destroy();
};

