#include "Direction.h"
#include "Common.h"

Direction RandomDirection()
{
	int randomNumber = RandomNumber(0, 4);
	switch (randomNumber)
	{
	case 0:
		return Direction::Up;
	case 1:
		return Direction::Down;
	case 2:
		return Direction::Left;
	case 3:
		return Direction::Right;
	default:
		return Direction::Up;
	}
}

Direction FlipDirection(Direction direction)
{
	switch (direction)
	{
	case Direction::Up:
		return Direction::Down;
	case Direction::Right:
		return Direction::Left;
	case Direction::Down:
		return Direction::Up;
	case Direction::Left:
		return Direction::Right;
	}
	return Direction::Up;
}
