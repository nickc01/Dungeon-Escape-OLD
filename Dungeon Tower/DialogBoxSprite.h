#pragma once
#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources

//Represents a blank dialog box that will be rendered to the center of the screen
class DialogBoxSprite : public Renderable
{
	//The resource texture of the dialog box
	static ResourceTexture dialogBoxTexture;

	//The sprite of the dialog box
	sf::Sprite dialogBoxSprite;
public:

	//Constructs a new dialog box
	DialogBoxSprite();



	//Called when the dialog box is to be rendered
	virtual void Render(sf::RenderWindow& window) override;

};

