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


template<typename type>
sf::Vector2<type> VectorInDirection(Direction direction, int scalar)
{
	switch (direction)
	{
	case Direction::Up:
		return sf::Vector2<type>(0, scalar);
	case Direction::Right:
		return sf::Vector2<type>(scalar, 0);
	case Direction::Down:
		return sf::Vector2<type>(0, -scalar);
	case Direction::Left:
		return sf::Vector2<type>(-scalar, 0);
	}
	return sf::Vector2<type>(0, 0);
}

