#pragma once

#include "Vector2.h"

class RectI
{
private:
	int topHeight;
	int bottomHeight;
	int leftWidth;
	int rightWidth;

public:
	int Left;
	int Right;
	int Top;
	int Bottom;

	Vector2I Center;

	RectI(Vector2I Center, Vector2I dimensions);

	bool Intersects(const RectI& B) const;

	Vector2I BottomLeft() const;
	Vector2I TopRight() const;

	int TopHeight() const;
	int BottomHeight() const;
	int LeftWidth() const;
	int RightWidth() const;

	int GetWidth() const;
	int GetHeight() const;
};

