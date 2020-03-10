#include "BackgroundTile.h"
#include "Renderer.h"


BackgroundTile::BackgroundTile(Vector2 position, wchar_t character, Color characterColor, Color backgroundColor,bool collidable) : 
	Sprite(position),
	character(character),
	color(characterColor),
	backgroundColor(backgroundColor),
	collidable(collidable)

{

}

void BackgroundTile::SetCharacter(wchar_t character)
{
	this->character = character;
}

wchar_t BackgroundTile::GetCharacter()
{
	return character;
}

void BackgroundTile::SetColor(Color color)
{
	this->color = color;
}

Color BackgroundTile::GetColor()
{
	return color;
}

void BackgroundTile::SetBackgroundColor(Color backgroundColor)
{
	this->backgroundColor = backgroundColor;
}

Color BackgroundTile::GetBackgroundColor()
{
	return backgroundColor;
}

bool BackgroundTile::IsCollidable() const
{
	return collidable;
}

int BackgroundTile::GetDisplayLayer() const
{
	return 0;
}

void BackgroundTile::Render() const
{
	Vector2 RenderPosition = Position - Renderer::CameraPosition + (Console::GetConsoleWindowSize() / 2);

	Console::SetColorAtPosition(RenderPosition, color, backgroundColor);
	Console::DrawCharacter(RenderPosition, character);
}
