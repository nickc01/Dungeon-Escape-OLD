#include "Sprite.h"
#include "Renderer.h"


Sprite::Sprite(const sf::Sprite& sprite) :
	sprite(sprite)
{
}

Sprite::Sprite(sf::Sprite&& sprite) :
	sprite(sprite)
{

}

void Sprite::SetSprite(sf::Sprite& sprite)
{
	this->sprite = sprite;
}

void Sprite::SetSprite(sf::Sprite&& sprite)
{
	this->sprite = sprite;
}

sf::Sprite& Sprite::GetSprite()
{
	return sprite;
}

const sf::Sprite& Sprite::GetSprite() const
{
	return sprite;
}

Vector2 Sprite::GetPosition() const
{
	return sprite.getPosition();
}

void Sprite::SetPosition(Vector2 position)
{
	sprite.setPosition(position);
}
