#pragma once

#include "AnimatedEntity.h"
#include "WorldMap.h"
#include "ResourceTexture.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Direction.h"
#include "Array2D.h"

class Player : public AnimatedEntity
{
	static Player* currentPlayer;

	static ResourceTexture playerSpriteSheet;
	//bool alive = true;
	bool moving = false;

	bool invincible = false;
	float invincibilityTimer = 0.0f;
	float flickerTimer = 0.0f;
	float orbSpawnTimer = 0.0f;

	Direction travelDirection = Direction::Up;


	int health = 5;

	void UpdateSprite();

public:

	static Player* GetCurrentPlayer();


	Player(const WorldMap& map, sf::Vector2f spawnPoint);

	Player(const Player& player) = delete;
	Player(Player&& player) = delete;
	Player& operator=(const Player& player) = delete;
	Player& operator=(Player&& player) = delete;


	Direction GetPlayerDirection() const;
	bool IsMoving() const;

	int GetHealth() const;
	void TakeHit();

	bool IsAlive() const;



	// Inherited via Animated Entity
	virtual void Update(sf::Time dt) override;

	virtual ~Player();
};

