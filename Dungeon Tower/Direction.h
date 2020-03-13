#pragma once


enum class Direction
{
	Up = 0,
	Right = 90,
	Down = 180,
	Left = 270
};


template<typename type = int>
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

template<typename type = int>
Direction RotateDirection(Direction source, type amount)
{
	type numDirection = static_cast<type>(source);

	numDirection += amount;

	numDirection %= 360;

	if (numDirection < 0)
	{
		numDirection += 360;
	}

	return static_cast<Direction>(static_cast<int>(numDirection));
}

