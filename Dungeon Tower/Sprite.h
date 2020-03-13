#pragma once
#include <SFML/Graphics.hpp>
#include "Common.h"
#include "Displayable.h"



class Sprite
{
protected:
	sf::Sprite sprite;
public:

	Sprite(const sf::Sprite& sprite);
	Sprite(sf::Sprite&& sprite);

	void SetSprite(sf::Sprite& sprite);
	void SetSprite(sf::Sprite&& sprite);
	sf::Sprite& GetSprite();
	const sf::Sprite& GetSprite() const;

	Vector2 GetPosition() const;
	void SetPosition(Vector2 position);
};

