#include "Door.h"
#include "resource.h"

Door* Door::currentDoor = nullptr;
ResourceTexture Door::doorTexture{ DOOR_SPRITE };

Door::Door(sf::Vector2f doorPosition) :
	doorSprite(doorTexture.GetTexture())
{
	doorSprite.setPosition(doorPosition);
	SetRenderLayer(5);
	currentDoor = this;
	EnableRendering();
}

Door* Door::GetDoor()
{
	return currentDoor;
}

const sf::Sprite& Door::GetSprite() const
{
	return doorSprite;
}

Door::~Door()
{
	EnableRendering(false);
	if (currentDoor == this)
	{
		currentDoor = nullptr;
	}
}

void Door::Render(sf::RenderWindow& window)
{
	window.draw(doorSprite);
}
