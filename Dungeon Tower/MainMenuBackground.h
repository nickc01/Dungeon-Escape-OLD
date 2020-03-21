#pragma once
#include "Renderable.h"
#include "ResourceTexture.h"

class MainMenuBackground : public Renderable
{
	static ResourceTexture backgroundTilesTexture;


	sf::Sprite backgroundSprite;

public:

	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;


	MainMenuBackground();

};

