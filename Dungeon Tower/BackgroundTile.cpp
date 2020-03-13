#include "BackgroundTile.h"
#include "Renderer.h"

BackgroundTile::BackgroundTile(const sf::Sprite& sprite, bool collidable) :
	sprite(sprite),
	collidable(collidable)
{

}

BackgroundTile::BackgroundTile(const sf::Sprite& sprite, Vector2I position, bool collidable) :
	BackgroundTile(sprite,collidable)
{
	this->sprite.setPosition(static_cast<Vector2>(position));
}

bool BackgroundTile::IsCollidable() const
{
	return collidable;
}

const sf::Sprite& BackgroundTile::GetSprite() const
{
	return sprite;
}

sf::Sprite& BackgroundTile::GetSprite()
{
	return sprite;
}

void BackgroundTile::SetSprite(const sf::Sprite& sprite)
{
	this->sprite = sf::Sprite(sprite);
}

std::shared_ptr<BackgroundTile> BackgroundTile::Create(const sf::Sprite& sprite, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,collidable));
}

std::shared_ptr<BackgroundTile> BackgroundTile::Create(const sf::Sprite& sprite, Vector2I position, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,position, collidable));
}
