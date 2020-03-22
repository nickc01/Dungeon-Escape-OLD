#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering

//Represents the four cardinal directions
enum class Direction
{
	Up = 0,
	Right = 90,
	Down = 180,
	Left = 270
};


//Returns a vector pointing in the specified direction with a specified scalar length
template<typename type = int>
sf::Vector2<type> VectorInDirection(Direction direction, int scalar)
{
	switch (direction)
	{
		//If the direction is up
	case Direction::Up:
		//Return a vector pointing in that direction
		return sf::Vector2<type>(0, scalar);
		//If the direction is right
	case Direction::Right:
		//Return a vector pointing in that direction
		return sf::Vector2<type>(scalar, 0);
		//If the direction is down
	case Direction::Down:
		//Return a vector pointing in that direction
		return sf::Vector2<type>(0, -scalar);
		//If the direction is left
	case Direction::Left:
		//Return a vector pointing in that direction
		return sf::Vector2<type>(-scalar, 0);
	}
	//A default if no matches were made
	return sf::Vector2<type>(0, 0);
}

//Rotates the direction by a specified amount of degrees. Only 90 degree turns are valid
template<typename type = int>
Direction RotateDirection(Direction source, type amount)
{
	//Conver the direction to a number in degrees
	type numDirection = static_cast<type>(source);

	//Rotate the direction by the specified amount
	numDirection += amount;

	//If the direction is greater than 360, then loop back around to 0
	numDirection %= 360;

	//If the direction is less than zero, add 360 to it
	if (numDirection < 0)
	{
		numDirection += 360;
	}

	//Convert the direction back into a Direction Enum
	return static_cast<Direction>(static_cast<int>(numDirection));
}

//Returns a random direction
Direction RandomDirection();

//Flips the specified direction by 180 degrees
Direction FlipDirection(Direction direction);

//Converts a vector to a direction if it aligns with any of the cardial directions
template<typename VectorType>
Direction DirectionFromVector(sf::Vector2<VectorType> vector)
{
	//Represents zero
	constexpr VectorType zero = static_cast<VectorType>(0);

	//If the vector is pointing up
	if (vector.x == zero && vector.y > zero)
	{
		//Return the Up Direction
		return Direction::Up;
	}
	//If the vector is pointing down
	else if (vector.x == zero && vector.y < zero)
	{
		//Return the Down Direction
		return Direction::Down;
	}
	//If the vector is pointing right
	else if (vector.x > zero && vector.y == zero)
	{
		//Return the Right Direction
		return Direction::Right;
	}
	//If the vector is pointing left
	else if (vector.x < zero && vector.y == zero)
	{
		//Return the Left Direction
		return Direction::Left;
	}
	//A default if no matches were made
	return Direction::Up;
}

//Converts a direction into a number in degrees
template<typename NumType = int>
NumType DirectionToDegrees(Direction direction)
{
	//Cast the direction enum into a number
	return static_cast<NumType>(static_cast<int>(direction));
}


