#include "Branch.h"
#include "Room.h"

using namespace std;

void Branch::CreateTiles()
{
	tiles.clear();

	Vector2 currentPoint = startPoint;

	tiles.push_back(make_shared<BackgroundTile>(currentPoint, L' ', Color::Black, Color::Green));

	for (int i = 0; i < firstLength; i++)
	{
		currentPoint += VectorInDirection(direction, 1);
		tiles.push_back(make_shared<BackgroundTile>(currentPoint, L' ', Color::Black, Color::Blue));
	}

	for (int i = 0; i < secondLength; i++)
	{
		currentPoint += VectorInDirection(midDirection, 1);
		tiles.push_back(make_shared<BackgroundTile>(currentPoint, L' ', Color::Black, Color::Yellow));
	}

	for (int i = 0; i < thirdLength; i++)
	{
		currentPoint += VectorInDirection(direction, 1);
		tiles.push_back(make_shared<BackgroundTile>(currentPoint, L' ', Color::Black, Color::Purple));
	}
}

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

const std::vector<std::shared_ptr<BackgroundTile>>& Branch::GetTiles()
{
	CreateTiles();
	return tiles;
}

int Branch::GetStartLength() const
{
	return firstLength;
}

Direction Branch::GetStartDirection() const
{
	return direction;
}

int Branch::GetMidLength() const
{
	return secondLength;
}

Direction Branch::GetMidDirection() const
{
	return midDirection;
}

int Branch::GetEndLength() const
{
	return thirdLength;
}

Direction Branch::GetEndDirection() const
{
	return direction;
}
