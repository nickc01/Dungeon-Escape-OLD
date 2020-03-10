#include "Sprite.h"
#include "Renderer.h"

/*Sprite::Sprite() : Sprite(Vector2(0, 0))
{

}

Sprite::Sprite(Vector2 position) : Position(position)
{
	Renderer::AddDisplayable(this);
}

Vector2 Sprite::GetPosition() const
{
	return Position;
}

Sprite::~Sprite()
{
	Renderer::RemoveDisplayable(this);
}*/

Sprite::Sprite() : Sprite(Vector2(0, 0)) {}

Sprite::Sprite(Vector2 position) : Displayable(), Position(position) {}

Vector2 Sprite::GetPosition() const
{
	return Position;
}
