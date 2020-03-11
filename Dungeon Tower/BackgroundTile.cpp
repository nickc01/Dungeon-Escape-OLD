#include "BackgroundTile.h"
#include "Renderer.h"

using namespace std;

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

/*int BackgroundTile::GetDisplayLayer() const
{
	return 0;
}*/

void BackgroundTile::Render() const
{
	Vector2 consoleSize = Console::GetConsoleWindowSize();

	Vector2 RenderPosition = Position - Renderer::CameraPosition + (consoleSize / 2);

	int consoleWidth = get<0>(consoleSize);
	int consoleHeight = get<1>(consoleSize);

	int x = get<0>(RenderPosition);
	int y = get<1>(RenderPosition);

	if (x < 0 || x >= consoleWidth || y < 0 || y >= consoleHeight)
	{
		return;
	}

	Console::SetColorAtPosition(RenderPosition, color, backgroundColor);
	Console::DrawCharacter(RenderPosition, character);
}
