#pragma once

#include "AnimatedEntity.h" //Contains the AnimatedEntity class for entities that are animated
#include "WorldMap.h" //Contains the WorldMap class
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <vector> //Contains std::vector for storing objects in an array
#include "Direction.h" //Contains the Direction Enum for specifying the direction
#include "Array2D.h" //Contains the Array2D class, which is for storing objects in a 2D array

//The main player in the game
class Player : public AnimatedEntity
{
	static Player* currentPlayer; //The singleton object for the player class

	static ResourceTexture playerSpriteSheet; //The texture resource for the player

	bool moving = false; //Whether the player is moving or not

	bool invincible = false; //Whether the player is invincible or not
	float invincibilityTimer = 0.0f; //Keeps track of how long the player is invincible for
	float flickerTimer = 0.0f; //Keeps track of how long the player should be flickering
	float orbSpawnTimer = 0.0f; //Keeps track of how fast the player should be spawning magic orbs

	Direction travelDirection = Direction::Up; //The direction the player is traveling in


	bool reachedDoor = false; //Whether the player has reached the door or not
	int health = 5; //The health of the player

	//Updates the player sprite
	void UpdateSprite();

public:

	//Gets the currently spawned player in the game
	static Player* GetCurrentPlayer();

	//Constructs a new player
	Player(const WorldMap& map, sf::Vector2f spawnPoint);

	//Deleting these four functions prevents the object from being copied or moved
	Player(const Player& player) = delete;
	Player(Player&& player) = delete;
	Player& operator=(const Player& player) = delete;
	Player& operator=(Player&& player) = delete;

	//Gets the player's traveling direction
	Direction GetPlayerDirection() const;
	//Returns whether the player is moving or not
	bool IsMoving() const;

	//Gets the health of the player
	int GetHealth() const;

	//Causes the player to take a hit and lose some health
	void TakeHit();

	//Whether the player is alive or not
	bool IsAlive() const;

	//Whether the player has reached the door or not
	bool ReachedTheDoor() const;

	//The update loop of the player
	virtual void Update(sf::Time dt) override;

	//Destructs the player
	virtual ~Player();
};

