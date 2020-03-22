#include "BackgroundTile.h" //Contains the BackgroundTile class

using namespace sf; //Prevents me from having to type sf everywhere

//Constructs a new background tile with the specified sprite
BackgroundTile::BackgroundTile(const Sprite& sprite, bool collidable) :
	sprite(sprite),
	collidable(collidable)
{

}

//Constructs a new background tile with the specified sprite and position
BackgroundTile::BackgroundTile(const Sprite& sprite, Vector2<int> position, bool collidable) :
	BackgroundTile(sprite,collidable)
{
	//Set the sprite's position
	this->sprite.setPosition(static_cast<Vector2<float>>(position));
}

//Returns whether the sprite is colidable or not
bool BackgroundTile::IsCollidable() const
{
	return collidable;
}

//Gets the sprite of the tile
const Sprite& BackgroundTile::GetSprite() const
{
	return sprite;
}

//Gets the sprite of the tile
Sprite& BackgroundTile::GetSprite()
{
	return sprite;
}

//Sets the sprite of the tile
void BackgroundTile::SetSprite(const Sprite& sprite)
{
	this->sprite = Sprite(sprite);
}

//Create a new shared_ptr of a background tile
std::shared_ptr<BackgroundTile> BackgroundTile::Create(const Sprite& sprite, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,collidable));
}

//Create a new shared_ptr of a background tile
std::shared_ptr<BackgroundTile> BackgroundTile::Create(const Sprite& sprite, Vector2<int> position, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,position, collidable));
}
