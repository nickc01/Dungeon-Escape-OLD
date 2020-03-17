#pragma once

#include <SFML/Graphics.hpp>

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

Direction RandomDirection();
Direction FlipDirection(Direction direction);

template<typename VectorType>
Direction DirectionFromVector(sf::Vector2<VectorType> vector)
{
	constexpr VectorType zero = static_cast<VectorType>(0);

	if (vector.x == zero && vector.y > zero)
	{
		return Direction::Up;
	}
	else if (vector.x == zero && vector.y < zero)
	{
		return Direction::Down;
	}
	else if (vector.x > zero && vector.y == zero)
	{
		return Direction::Right;
	}
	else if (vector.x < zero && vector.y == zero)
	{
		return Direction::Left;
	}

	return Direction::Up;
}

template<typename NumType = int>
NumType DirectionToDegrees(Direction direction)
{
	return static_cast<NumType>(static_cast<int>(direction));
}


