#pragma once

#include "Vector2.h"

class Rect
{
public:
	int Left;
	int Right;
	int Top;
	int Bottom;

	Rect(Vector2 Center, Vector2 dimensions);

	bool Intersects(const Rect& B) const;

	Vector2 BottomLeft();
	Vector2 TopRight();
};

