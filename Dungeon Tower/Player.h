#pragma once

#include "Renderable.h"
#include "WorldMap.h"
#include "ResourceTexture.h"
#include "UpdateReceiver.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Direction.h"
#include "Array2D.h"

class Player : public Renderable, public UpdateReceiver
{
	float animationTimer = 0.0f;

	static ResourceTexture playerSpriteSheet;

	const WorldMap& map;
	bool alive = true;
	bool moving = false;

	bool indexForward = true;
	int spriteIndex = 0;
	Direction travelDirection = Direction::Up;

	std::vector<sf::Sprite> leftSprites;
	std::vector<sf::Sprite> upSprites;
	std::vector<sf::Sprite> downSprites;
	std::vector<sf::Sprite> rightSprites;

	sf::Sprite* currentSprite = nullptr;
	//sf::Sprite playerSprite;

	void UpdateSprite();

	Array2D<BackgroundTile*> GetTilesAroundPlayer() const;

public:

	Player(const WorldMap& map, sf::Vector2f spawnPoint);

	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) const override;

	// Inherited via UpdateReceiver
	virtual void Update(sf::Time dt) override;

	void SetSprite(sf::Sprite& sprite);
	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;

	Direction GetPlayerDirection() const;

	void Move(float x, float y);
	void Move(sf::Vector2f direction);

	void GetPlayerSpriteEdge(Direction direction, bool worldCoords = true) const;
};

