#pragma once
#include "Common.h"

#include "Displayable.h"


class Sprite : public Displayable
{
protected:
	Vector2 Position;
public:

	Sprite();
	Sprite(Vector2 position);

	Vector2 GetPosition() const;
};

