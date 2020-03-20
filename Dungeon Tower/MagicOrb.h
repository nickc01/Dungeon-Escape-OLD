#pragma once
#include "ResourceTexture.h"
#include <list>
#include <SFML/Graphics.hpp>
#include "Direction.h"
#include <mutex>
#include "Entity.h"
#include "WorldMap.h"

class MagicOrb : public Entity
{
	static ResourceTexture orbTexture;
	static std::recursive_mutex orbsMutex;
	static std::list<std::shared_ptr<MagicOrb>> SpawnedOrbs;


	float spawnInvincibilityTimer = 0.0f;
	float lifeTimeCounter = 0.0f;
	const WorldMap& map;
	sf::Sprite orbSprite;
	sf::Vector2f direction;
	bool alive = true;


	MagicOrb(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction);

	// Inherited via UpdateReceiver
	virtual void Update(sf::Time dt) override;

	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;

public:

	MagicOrb(const MagicOrb& copy) = delete;
	MagicOrb(MagicOrb&& move) = delete;

	MagicOrb& operator=(const MagicOrb& copy) = delete;
	MagicOrb& operator=(MagicOrb&& move) = delete;

	static std::recursive_mutex& GetOrbListMutex();
	static const std::list<std::shared_ptr<MagicOrb>>& GetAllOrbs();


	static std::shared_ptr<MagicOrb> Fire(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction);
	static std::shared_ptr<MagicOrb> Fire(const WorldMap& map, sf::Vector2f spawnPoint, Direction direction);

	void Destroy();

	//~MagicOrb();
};

