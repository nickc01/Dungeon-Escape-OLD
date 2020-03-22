#include "Door.h" //Contains the Door class, which is where the player enters to win the game
#include "resource.h" //Contains the IDs of the game's resources

Door* Door::currentDoor = nullptr; //A singleton that represents the current door in the world map
ResourceTexture Door::doorTexture{ DOOR_SPRITE }; //The door's resource texture

//Constructs a new door
Door::Door(sf::Vector2f doorPosition) :
	//Set the doors sprite to the resource texture
	doorSprite(doorTexture.GetTexture())
{
	//Set the sprite's position
	doorSprite.setPosition(doorPosition);
	//Set it's render layer
	SetRenderLayer(5);
	//Update the singleton
	currentDoor = this;
	//Enable rendering
	EnableRendering();
}


//Gets the singleton that represents the door in the world map
Door* Door::GetDoor()
{
	return currentDoor;
}

//Gets the sprite of the door
const sf::Sprite& Door::GetSprite() const
{
	return doorSprite;
}

Door::~Door()
{
	//If the singleton is set to itself
	if (currentDoor == this)
	{
		//Reset it to null
		currentDoor = nullptr;
	}
}

//Called when the dialog box is to be rendered
void Door::Render(sf::RenderWindow& window)
{
	//Draw the door sprite
	window.draw(doorSprite);
}
