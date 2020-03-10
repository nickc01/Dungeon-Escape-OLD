#include "Direction.h"

Direction RotateDirection(Direction source,int amount)
{
	int numDirection = static_cast<int>(source);

	numDirection += amount;

	numDirection %= 360;

	if (numDirection < 0)
	{
		numDirection += 360;
	}

	return static_cast<Direction>(numDirection);
}

Vector2 VectorInDirection(Direction direction, int scalar)
{
	switch (direction)
	{
	case Direction::Up:
		return Vector2(0, scalar);
	case Direction::Right:
		return Vector2(scalar, 0);
	case Direction::Down:
		return Vector2(0, -scalar);
	case Direction::Left:
		return Vector2(-scalar, 0);
	}
	return Vector2(0, 0);
}
