#pragma once

#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "Console.h"
#include <memory>
class BackgroundTile
{
	sf::Sprite sprite;
	bool collidable;
	BackgroundTile(const sf::Sprite& sprite, bool collidable = false);
	BackgroundTile(const sf::Sprite& sprite, Vector2I position, bool collidable = false);

public:

	bool IsCollidable() const;


	const sf::Sprite& GetSprite() const;
	sf::Sprite& GetSprite();
	void SetSprite(const sf::Sprite& sprite);


	static std::shared_ptr<BackgroundTile> Create(const sf::Sprite& sprite, bool collidable = false);
	static std::shared_ptr<BackgroundTile> Create(const sf::Sprite& sprite, Vector2I position, bool collidable = false);
};

