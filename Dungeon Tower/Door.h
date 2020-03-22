#pragma once
#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources


//Represents the exit door that the player needs to get to in order to win
class Door : public Renderable
{
	static Door* currentDoor; //A singleton that represents the current door in the world map
	static ResourceTexture doorTexture; //The door's resource texture

	sf::Sprite doorSprite; //The door's sprite

	//Called when the dialog box is to be rendered
	virtual void Render(sf::RenderWindow& window) override;

public:

	//Constructs a new door
	Door(sf::Vector2f doorPosition);

	//Gets the singleton that represents the door in the world map
	static Door* GetDoor();

	//Gets the sprite of the door
	const sf::Sprite& GetSprite() const;

	//Deleting these four functions prevents the object from being copied or moved
	Door(const Door&) = delete;
	Door(Door&&) = delete;
	Door& operator=(const Door&) = delete;
	Door& operator=(Door&&) = delete;

	//The destructor for the door
	virtual ~Door();
};

