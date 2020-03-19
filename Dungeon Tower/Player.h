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
	bool alive = true;
	bool moving = false;

	Direction travelDirection = Direction::Up;


	void UpdateSprite();

public:

	static const Player* GetCurrentPlayer();


	Player(const WorldMap& map, sf::Vector2f spawnPoint);

	Player(const Player& player) = delete;
	Player(Player&& player) = delete;
	Player& operator=(const Player& player) = delete;
	Player& operator=(Player&& player) = delete;


	Direction GetPlayerDirection() const;
	bool IsMoving() const;



	// Inherited via Animated Entity
	virtual void Update(sf::Time dt) override;

	virtual ~Player();
};

