#pragma once
#include "Renderable.h"
#include "ResourceTexture.h"
#include "Player.h"

class HeartDisplay : public Renderable
{
	static ResourceTexture heartTexture;

	const Player& player;
	sf::Sprite heartSprite;

public:

	HeartDisplay(const Player& player);

	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;
};