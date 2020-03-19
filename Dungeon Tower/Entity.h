#pragma once

#include "Renderable.h"
#include "UpdateReceiver.h"
#include "WorldMap.h"
#include "Direction.h"


class Entity : public Renderable, public UpdateReceiver
{
	const WorldMap& map;
	bool renderingEnabled;

	sf::Sprite* sprite = nullptr;


	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) const override;

protected:
	Array2D<BackgroundTile*> GetTilesAroundEntity() const;

	// Inherited via UpdateReceiver
	virtual void Update(sf::Time dt) override = 0;
public:

	Entity(const WorldMap& map, bool renderingEnabled = true,bool enableUpdating = true);
	Entity(const WorldMap& map,sf::Sprite* sprite, bool renderingEnabled = true, bool enableUpdating = true);

	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	void SetSprite(sf::Sprite* sprite);

	bool GetCollisionMode();
	void SetCollisionMode(bool enabled);

	void Move(float x, float y);
	void Move(sf::Vector2f direction);
	void Move(Direction direction, int scalar);

	static void MoveCameraTo(sf::Vector2f position, float lerpAmount = 1.0f);

};

