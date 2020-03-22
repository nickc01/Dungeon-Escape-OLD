#pragma once
#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources
#include "Player.h" //Contains the Player class that represents the player

//Used to display how many lives the player has left
class HeartDisplay : public Renderable
{
	static ResourceTexture heartTexture; //The texture resource for the heart display

	//The player the heart display is bound to
	const Player& player;

	//The heart display sprite
	sf::Sprite heartSprite;

public:

	//Constructs a new heart display
	HeartDisplay(const Player& player);

	//Renders the heart display to the screen
	virtual void Render(sf::RenderWindow& window) override;
};