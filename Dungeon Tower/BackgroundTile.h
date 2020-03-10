#pragma once
#include "Sprite.h"
#include "Console.h"
class BackgroundTile : public Sprite
{
	wchar_t character;
	Color color;
	Color backgroundColor;
	bool collidable;

public:
	BackgroundTile(Vector2 position,wchar_t character, Color characterColor, Color backgroundColor = Black, bool collidable = false);


	void SetCharacter(wchar_t character);
	wchar_t GetCharacter();

	void SetColor(Color color);
	Color GetColor();

	void SetBackgroundColor(Color backgroundColor);
	Color GetBackgroundColor();

	bool IsCollidable() const;

	// Inherited via Sprite
	virtual int GetDisplayLayer() const override;
	virtual void Render() const override;
};

