#include "Branch.h"
#include "Room.h"

using namespace std;
using namespace sf;

void Branch::CreateTiles()
{
	tiles.clear();

	Vector2<int> currentPoint = startPoint;

	auto leftDirection = RotateDirection(direction, -90);
	auto rightDirection = RotateDirection(direction, 90);

	auto midLeftDirection = RotateDirection(midDirection, -90);
	auto midRightDirection = RotateDirection(midDirection, 90);

	auto leftVector = VectorInDirection(leftDirection, 1);
	auto rightVector = VectorInDirection(rightDirection, 1);
	auto forwardVector = VectorInDirection(direction, 1);

	auto midLeftVector = VectorInDirection(midLeftDirection, 1);
	auto midRightVector = VectorInDirection(midRightDirection, 1);
	auto midForwardVector = VectorInDirection(midDirection, 1);


	BuildJointPiece(direction, currentPoint);

	/*tiles.push_back(BackgroundTile::Create(Common::Sprites::centerPiece2,currentPoint));

	tiles[0]->GetSprite().setColor(Color(0, 255, 255));

	tiles.push_back(BackgroundTile::Create(Common::GetJointPiece(leftDirection, direction), currentPoint + rightVector));
	tiles.push_back(BackgroundTile::Create(Common::GetJointPiece(rightDirection,direction),currentPoint + leftVector));*/
	//tiles.push_back(BackgroundTile::Create());


	for (int i = 1; i < firstLength; i++)
	{
		currentPoint += forwardVector;
		BuildStraightPiece(direction, currentPoint);
		/*tiles.push_back(BackgroundTile::Create(Common::GetSideSprite(leftDirection),currentPoint + leftVector));
		tiles.push_back(BackgroundTile::Create(Common::Sprites::centerPiece1, currentPoint));
		tiles.push_back(BackgroundTile::Create(Common::GetSideSprite(rightDirection), currentPoint + rightVector));*/
	}

	currentPoint += forwardVector;

	BuildCurvePiece(direction, midDirection, currentPoint);

	currentPoint += midForwardVector;

	for (int i = 2; i < secondLength; i++)
	{
		currentPoint += midForwardVector;

		BuildStraightPiece(midDirection, currentPoint);

		//BuildTile(Common::Sprites::centerPiece1, currentPoint);
		//currentPoint += VectorInDirection<int>(midDirection, 1);

		//tiles.push_back(BackgroundTile::Create(Common::Sprites::centerPiece1, currentPoint));
	}

	currentPoint += midForwardVector;

	BuildCurvePiece(midDirection, direction, currentPoint);

	currentPoint += forwardVector;

	for (int i = 2; i < thirdLength; i++)
	{
		currentPoint += forwardVector;
		BuildStraightPiece(direction, currentPoint);
		//tiles.push_back(BackgroundTile::Create(Common::Sprites::centerPiece1, currentPoint));
	}

	currentPoint += forwardVector;

	BuildJointPiece(RotateDirection(direction,180), currentPoint);
}

void Branch::BuildTile(const Sprite& sprite, Vector2<int> position)
{
	tiles.push_back(BackgroundTile::Create(sprite, position));
}

void Branch::BuildJointPiece(Direction direction, Vector2i position)
{
	auto leftDirection = RotateDirection(direction, -90);
	auto rightDirection = RotateDirection(direction, 90);

	auto leftVector = VectorInDirection(leftDirection, 1);
	auto rightVector = VectorInDirection(rightDirection, 1);
	auto forwardVector = VectorInDirection(direction, 1);

	BuildTile(Common::Sprites::centerPiece2,position);
	BuildTile(Common::GetJointPiece(leftDirection, direction), position + rightVector);
	BuildTile(Common::GetJointPiece(rightDirection, direction), position + leftVector);

}

void Branch::BuildStraightPiece(Direction direction, sf::Vector2i position)
{
	auto leftDirection = RotateDirection(direction, -90);
	auto rightDirection = RotateDirection(direction, 90);

	auto leftVector = VectorInDirection(leftDirection, 1);
	auto rightVector = VectorInDirection(rightDirection, 1);

	BuildTile(Common::GetSideSprite(leftDirection), position + leftVector);
	BuildTile(Common::Sprites::centerPiece2, position);
	BuildTile(Common::GetSideSprite(rightDirection), position + rightVector);
}

void Branch::BuildCurvePiece(Direction from, Direction to, sf::Vector2i position)
{
	auto leftDirection = RotateDirection(from, -90);
	auto rightDirection = RotateDirection(from, 90);

	auto destLeftDirection = RotateDirection(to, -90);
	auto destRightDirection = RotateDirection(to, 90);

	auto leftVector = VectorInDirection(leftDirection, 1);
	auto rightVector = VectorInDirection(rightDirection, 1);
	auto forwardVector = VectorInDirection(from, 1);

	auto destLeftVector = VectorInDirection(destLeftDirection, 1);
	auto destRightVector = VectorInDirection(destRightDirection, 1);
	auto destForwardVector = VectorInDirection(to, 1);

	BuildTile(Common::Sprites::centerPiece2, position);

	if (to == leftDirection)
	{
		BuildTile(Common::GetSideSprite(rightDirection), position + rightVector);
		BuildTile(Common::GetCornerSprite(from, rightDirection), position + rightVector + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector + leftVector);

		BuildTile(Common::GetJointPiece(from, to), position - forwardVector + leftVector);
	}
	else if (to == rightDirection)
	{
		BuildTile(Common::GetSideSprite(leftDirection), position + leftVector);
		BuildTile(Common::GetCornerSprite(from, leftDirection), position + leftVector + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector + rightVector);

		BuildTile(Common::GetJointPiece(from, to), position - forwardVector + rightVector);
	}

	BuildTile(Common::Sprites::centerPiece2, position + destForwardVector);
}

Branch::Branch(Direction direction)
{
	this->direction = direction;
	firstLength = RandomNumber(4, 6);
	secondLength = RandomNumber(2, 7);
	thirdLength = RandomNumber(4, 8);

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

std::shared_ptr<Room> Branch::GetDestinationRoom()
{
	return destinationRoom;
}

void Branch::SetDestinationRoom(std::shared_ptr<Room> room)
{
	destinationRoom = room;
}

Direction Branch::GetDirection()
{
	return direction;
}

void Branch::SetStartPoint(Vector2<int> point)
{
	updated = true;
	startPoint = point;
}

Vector2<int> Branch::GetStartPoint()
{
	return startPoint;
}

Vector2<int> Branch::GetDestinationPoint()
{
	return startPoint + VectorInDirection<int>(direction, firstLength) + VectorInDirection<int>(midDirection, secondLength) + VectorInDirection<int>(direction, thirdLength);
}

const std::vector<std::shared_ptr<BackgroundTile>>& Branch::GetTiles()
{
	if (updated)
	{
		CreateTiles();
		updated = false;
	}
	return tiles;
}

bool Branch::Intersects(Branch& other)
{
	for (auto& tileA : GetTiles())
	{
		for (auto& tileB : other.GetTiles())
		{
			if (tileA == nullptr || tileB == nullptr)
			{
				continue;
			}
			else if (Common::SpritesIntersect(tileA->GetSprite(),tileB->GetSprite()))
			{
				return true;
			}
		}
	}
	return false;
}

bool Branch::CheckForCollisions(Room* TopRoom)
{
	for (auto room : TopRoom->GetAllConnectedRooms())
	{
		for (auto& tile : GetTiles())
		{
			room->Intersects(*tile);
		}
	}

	for (auto branch : TopRoom->GetAllConnectedBranches())
	{
		if (branch == nullptr || branch == this)
		{
			continue;
		}
		else if (Intersects(*branch))
		{
			return true;
		}
	}
	return false;
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
