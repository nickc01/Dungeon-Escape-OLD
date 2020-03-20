#pragma once

#include "Renderable.h"
#include "UpdateReceiver.h"
#include "WorldMap.h"
#include "Direction.h"


class Entity : public Renderable, public UpdateReceiver
{
	const WorldMap& map;
	bool collisionEnabled;
	sf::Rect<float> hitbox = sf::Rect<float>(0.0f, 0.0f, 0.0f, 0.0f);

	sf::Sprite* sprite = nullptr;


	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;

protected:
	Array2D<BackgroundTile*> GetTilesAroundEntity() const;

	// Inherited via UpdateReceiver
	virtual void Update(sf::Time dt) override = 0;
public:

	Entity(const WorldMap& map, bool collisionEnabled = true);
	Entity(const WorldMap& map,sf::Sprite* sprite, bool collisionEnabled = true);

	sf::Sprite* GetSprite();
	const sf::Sprite* GetSprite() const;
	void SetSprite(sf::Sprite* sprite);

	bool GetCollisionMode();
	void SetCollisionMode(bool enabled);
	sf::Rect<float> GetHitBox() const;
	sf::Rect<float> GetHitBoxGlobalBounds() const;
	void SetHitbox(sf::Rect<float> hitbox);

	void Move(float x, float y);
	void Move(sf::Vector2f direction);
	void Move(Direction direction, int scalar);

	float GetDistanceToWall(Direction direction) const;
	float GetDistanceToWall(Direction direction, sf::Vector2f offset) const;
	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction) const;
	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, sf::Vector2f offset) const;

	bool IsTouchingWall(sf::Vector2f offset = sf::Vector2f(0, 0)) const;
	//bool IsTouchingWall(sf::Rect<float> hitbox = sf::Rect<float>(0, 0, 0, 0)) const;
	//bool IsTouchingWall(sf::Vector2f offset) const;

	static void MoveCameraTo(sf::Vector2f position, float lerpAmount = 1.0f);

	const WorldMap& GetMap() const;

};

