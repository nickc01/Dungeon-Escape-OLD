#include "BackgroundTile.h"

using namespace sf;

BackgroundTile::BackgroundTile(const Sprite& sprite, bool collidable) :
	sprite(sprite),
	collidable(collidable)
{

}

BackgroundTile::BackgroundTile(const Sprite& sprite, Vector2<int> position, bool collidable) :
	BackgroundTile(sprite,collidable)
{
	this->sprite.setPosition(static_cast<Vector2<float>>(position));
}

bool BackgroundTile::IsCollidable() const
{
	return collidable;
}

const Sprite& BackgroundTile::GetSprite() const
{
	return sprite;
}

Sprite& BackgroundTile::GetSprite()
{
	return sprite;
}

void BackgroundTile::SetSprite(const Sprite& sprite)
{
	this->sprite = Sprite(sprite);
}

std::shared_ptr<BackgroundTile> BackgroundTile::Create(const Sprite& sprite, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,collidable));
}

std::shared_ptr<BackgroundTile> BackgroundTile::Create(const Sprite& sprite, Vector2<int> position, bool collidable)
{
	return std::shared_ptr<BackgroundTile>(new BackgroundTile(sprite,position, collidable));
}
