#pragma once
#include "Renderable.h"
#include "ResourceTexture.h"

class DialogBoxSprite : public Renderable
{
	static ResourceTexture dialogBoxTexture;


	sf::Sprite dialogBoxSprite;
public:


	DialogBoxSprite();



	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;

};

