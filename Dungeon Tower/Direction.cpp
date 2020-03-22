#include "Direction.h" //Contains the Direction Enum for specifying the direction
#include "Common.h" //Contains many common game functions and variables

//Returns a random direction
Direction RandomDirection()
{
	//Get a random number between 0 and 3
	int randomNumber = Common::RandomNumber(0, 4);
	switch (randomNumber)
	{
		//If the number is zero
	case 0:
		//Return the up direction
		return Direction::Up;
		//If the number is one
	case 1:
		//Return the down direction
		return Direction::Down;
		//If the number is two
	case 2:
		//Return the left direction
		return Direction::Left;
		//If the number is three
	case 3:
		//Return the right direction
		return Direction::Right;
		//If none are true
	default:
		//Return a default direction
		return Direction::Up;
	}
}

//Flips the specified direction by 180 degrees
Direction FlipDirection(Direction direction)
{
	return RotateDirection(direction, 180);
}
