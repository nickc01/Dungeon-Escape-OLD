#include "Branch.h"
#include "Room.h"

Branch::Branch(Direction direction)
{
	this->direction = direction;
	firstLength = RandomNumber(2, 6);
	secondLength = RandomNumber(1, 7);
	thirdLength = RandomNumber(3, 8);

	int directionNumber = RandomNumber(0, 2);
	if (directionNumber == 1)
	{
		midDirection = RotateDirection(direction, 90);
	}
	else
	{
		midDirection = RotateDirection(direction, -90);
	}
}

/*std::shared_ptr<Room> Branch::GetSourceRoom()
{
	return sourceRoom;
}*/

std::shared_ptr<Room> Branch::GetDestinationRoom()
{
	return destinationRoom;
}

/*void Branch::SetSourceRoom(std::shared_ptr<Room> room)
{
	sourceRoom = room;
}*/

void Branch::SetDestinationRoom(std::shared_ptr<Room> room)
{
	destinationRoom = room;
}

Direction Branch::GetDirection()
{
	return direction;
}

void Branch::SetStartPoint(Vector2 point)
{
	startPoint = point;
}

Vector2 Branch::GetStartPoint()
{
	return startPoint;
}

Vector2 Branch::GetDestinationPoint()
{
	return startPoint + VectorInDirection(direction, firstLength) + VectorInDirection(midDirection, secondLength) + VectorInDirection(direction, thirdLength);
}
