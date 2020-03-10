#pragma once
#include "Vector2.h"


enum class Direction
{
	Up = 0,
	Right = 90,
	Down = 180,
	Left = 270
};



Direction RotateDirection(Direction source, int amount);

Vector2 VectorInDirection(Direction direction, int scalar);

