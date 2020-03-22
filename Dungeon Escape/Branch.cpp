#include "Branch.h" //Contains the Branch class for creating paths to connect rooms
#include "Room.h" //Contains the Room class that represents individual rooms in the game

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

//Creates all the tiles that make up the branch
void Branch::CreateTiles()
{
	//Clear the tile list of there are existing tiles in it
	tiles.clear();

	//Start at the start point
	Vector2<int> currentPoint = startPoint;

	//Get the leftwards direction from the main
	auto leftDirection = RotateDirection(direction, -90);
	//Get the rightwards direction from the main
	auto rightDirection = RotateDirection(direction, 90);

	//Get the leftwards direction from the midSection direction
	auto midLeftDirection = RotateDirection(midSectionDirection, -90);
	//Get the rightwards direction from the midSection direction
	auto midRightDirection = RotateDirection(midSectionDirection, 90);

	//Get the vector that points in the leftwards direction from the main
	auto leftVector = VectorInDirection(leftDirection, 1);
	//Get the vector that points in the rightwards direction from the main
	auto rightVector = VectorInDirection(rightDirection, 1);
	//Get the vector that points in the forwards direction from the main
	auto forwardVector = VectorInDirection(direction, 1);

	//Get the vector that points in the leftwards direction from the midSection direction
	auto midLeftVector = VectorInDirection(midLeftDirection, 1);
	//Get the vector that points in the rightwards direction from the midSection direction
	auto midRightVector = VectorInDirection(midRightDirection, 1);
	//Get the vector that points in the forwards direction from the midSection direction
	auto midForwardVector = VectorInDirection(midSectionDirection, 1);

	//Build a joint piece that connects the start room and the branch path together
	BuildJointPiece(direction, currentPoint);

	//Create straight pieces to build the first section of the path
	for (int i = 1; i < startLength; i++)
	{
		//Go forwards one tile
		currentPoint += forwardVector;

		//Create the straight piece
		BuildStraightPiece(direction, currentPoint);
	}
	//Go forwards one tile
	currentPoint += forwardVector;

	//Create a curved piece that points towards the mid section direction
	BuildCurvePiece(direction, midSectionDirection, currentPoint);

	//Go forwards one tile
	currentPoint += midForwardVector;

	//Create straight pieces to build the second section of the path
	for (int i = 2; i < midLength; i++)
	{
		//Go forwards one tile
		currentPoint += midForwardVector;

		//Create the straight piece
		BuildStraightPiece(midSectionDirection, currentPoint);
	}

	//Go forwards one tile
	currentPoint += midForwardVector;

	//Build a curve piece that points towards the main direction to link to the destination room
	BuildCurvePiece(midSectionDirection, direction, currentPoint);

	//Go forwards one tile
	currentPoint += forwardVector;

	//Create straight pieces to build the final section of the path
	for (int i = 2; i < endLength; i++)
	{
		//Go forwards one tile
		currentPoint += forwardVector;
		//Create the straight piece
		BuildStraightPiece(direction, currentPoint);
	}

	//Go forwards one tile
	currentPoint += forwardVector;

	//Build the joint piece that connects the destination room with the branch path
	BuildJointPiece(RotateDirection(direction,180), currentPoint);
}

//Builds a single tile
void Branch::BuildTile(const Sprite& sprite, Vector2<int> position)
{
	//Add a new background tile to the list of tiles
	tiles.push_back(BackgroundTile::Create(sprite, position));
}

void Branch::BuildJointPiece(Direction direction, Vector2i position)
{
	//Get the leftwards direction
	auto leftDirection = RotateDirection(direction, -90);
	//Get the rightwards direction
	auto rightDirection = RotateDirection(direction, 90);

	//Get the vector that points in the leftwards direction
	auto leftVector = VectorInDirection(leftDirection, 1);
	//Get the vector that points in the rightwards direction
	auto rightVector = VectorInDirection(rightDirection, 1);
	//Get the vector that points in the forwards direction
	auto forwardVector = VectorInDirection(direction, 1);

	//Build the center of the joint
	BuildTile(Common::Sprites::centerPiece2,position);

	//Build the left side of the joint
	BuildTile(Common::GetJointPiece(leftDirection, direction), position + rightVector);

	//Build the right side of the joint
	BuildTile(Common::GetJointPiece(rightDirection, direction), position + leftVector);

}

//Builds a new straight piece at the specified position
void Branch::BuildStraightPiece(Direction direction, sf::Vector2i position)
{
	//Get the leftwards direction
	auto leftDirection = RotateDirection(direction, -90);
	//Get the rightwards direction
	auto rightDirection = RotateDirection(direction, 90);

	//Get the vector that points in the leftwards direction
	auto leftVector = VectorInDirection(leftDirection, 1);
	//Get the vector that points in the rightwards direction
	auto rightVector = VectorInDirection(rightDirection, 1);

	//Build the left side of the straight piece
	BuildTile(Common::GetSideSprite(leftDirection), position + leftVector);
	//Build the center part of the straight piece
	BuildTile(Common::Sprites::centerPiece2, position);
	//Build the right side of the straight piece
	BuildTile(Common::GetSideSprite(rightDirection), position + rightVector);
}

//Builds a curve piece at the specified position
void Branch::BuildCurvePiece(Direction from, Direction to, sf::Vector2i position)
{
	//Get the direction to the left of the source direction
	auto leftDirection = RotateDirection(from, -90);

	//Get the direction to the right of the source direction
	auto rightDirection = RotateDirection(from, 90);

	//Get the direction to the left of the destination direction
	auto destLeftDirection = RotateDirection(to, -90);

	//Get the direction to the right of the destination direction
	auto destRightDirection = RotateDirection(to, 90);

	//Get the vector that points to the left of the source direction
	auto leftVector = VectorInDirection(leftDirection, 1);
	//Get the vector that points to the right of the source direction
	auto rightVector = VectorInDirection(rightDirection, 1);
	//Get the vector that points towards the source direction
	auto forwardVector = VectorInDirection(from, 1);

	//Get the vector that points to the left of the destination direction
	auto destLeftVector = VectorInDirection(destLeftDirection, 1);
	//Get the vector that points to the right of the destination direction
	auto destRightVector = VectorInDirection(destRightDirection, 1);
	//Get the vector that points towards the destination direction
	auto destForwardVector = VectorInDirection(to, 1);

	//Build the center part of the curve piece
	BuildTile(Common::Sprites::centerPiece2, position);

	//If we are curving towards the left
	if (to == leftDirection)
	{
		//Build the entire outer edge of the curve piece
		BuildTile(Common::GetSideSprite(rightDirection), position + rightVector);
		BuildTile(Common::GetCornerSprite(from, rightDirection), position + rightVector + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector + leftVector);

		//Build the inside of the curve piece
		BuildTile(Common::GetJointPiece(from, to), position - forwardVector + leftVector);
	}
	//If we are curving towards the right
	else if (to == rightDirection)
	{
		//Build the entire outer edge of the curve piece
		BuildTile(Common::GetSideSprite(leftDirection), position + leftVector);
		BuildTile(Common::GetCornerSprite(from, leftDirection), position + leftVector + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector);
		BuildTile(Common::GetSideSprite(from), position + forwardVector + rightVector);

		//Build the inside of the curve piece
		BuildTile(Common::GetJointPiece(from, to), position - forwardVector + rightVector);
	}

	//Build the next tile forward in the destination direction
	BuildTile(Common::Sprites::centerPiece2, position + destForwardVector);
}

//Constructs a new branch pointing in the specified direction
Branch::Branch(Direction direction)
{
	//Set the branch's direction
	this->direction = direction;
	
	//Set the section lengths, each with a random length
	startLength = Common::RandomNumber(4, 6);
	midLength = Common::RandomNumber(2, 7);
	endLength = Common::RandomNumber(4, 8);

	//Get a random number that is either 0 or 1
	int directionNumber = Common::RandomNumber(0, 2);
	
	//If the number is 1
	if (directionNumber == 1)
	{
		//Set the middle section to go to the right
		midSectionDirection = RotateDirection(direction, 90);
	}
	//If the number is 0
	else
	{
		//Set the middle section to go to the left
		midSectionDirection = RotateDirection(direction, -90);
	}
}

//Gets the pointer to the destination room
std::shared_ptr<Room> Branch::GetDestinationRoom()
{
	return destinationRoom;
}

//Sets the pointer to the destination room
void Branch::SetDestinationRoom(std::shared_ptr<Room> room)
{
	destinationRoom = room;
}

//Gets the main direction the branch is traveling in
Direction Branch::GetDirection()
{
	return direction;
}

//Sets the starting point for the branch
void Branch::SetStartPoint(Vector2<int> point)
{
	//Specify that the start point has been updated
	startPointUpdated = true;
	//Update the point
	startPoint = point;
}

//Gets the starting point of the branch
Vector2<int> Branch::GetStartPoint()
{
	return startPoint;
}

//Gets the destination point of the branch
Vector2<int> Branch::GetDestinationPoint()
{
	//Add up all the sections together to get the destination point
	return startPoint + VectorInDirection<int>(direction, startLength) + VectorInDirection<int>(midSectionDirection, midLength) + VectorInDirection<int>(direction, endLength);
}

//Gets all the tiles that make up the branch
const std::vector<std::shared_ptr<BackgroundTile>>& Branch::GetTiles()
{
	//If the start point has been updated
	if (startPointUpdated)
	{
		//Create the new set of tiles
		CreateTiles();
		startPointUpdated = false;
	}
	return tiles;
}

//Checks to see if a branch overlaps another one
bool Branch::Intersects(Branch& other)
{
	//Loop over all the tiles in the source branch
	for (auto& tileA : GetTiles())
	{
		//Loop over all the tiles in the other branch
		for (auto& tileB : other.GetTiles())
		{
			//If either of the tiles don't exist
			if (tileA == nullptr || tileB == nullptr)
			{
				//Continue to the next set of tiles
				continue;
			}
			//If the tiles intersect each other
			else if (Common::SpritesIntersect(tileA->GetSprite(),tileB->GetSprite(),true))
			{
				//An intersection has occured, return true
				return true;
			}
		}
	}
	//No intersections have occured, return false
	return false;
}

//Checks to see if the branch is colliding with any of the rooms or branches in the room hierarchy
bool Branch::CheckForCollisions(Room* TopRoom)
{
	//Loop over all the rooms in the hierarchy
	for (auto room : TopRoom->GetAllRoomsInHierarchy())
	{
		//Loop over all the tiles in each room
		for (auto& tile : GetTiles())
		{
			//Check if the room intersects with any of the branch's tiles
			if (room->Intersects(*tile))
			{
				//If it does, return true
				return true;
			}
		}
	}

	//Loop over all of the branches in the room hierarchy
	for (auto branch : TopRoom->GetAllBranchesInHierarchy())
	{
		//If the branch is null or if the branch is equal to the branch we are trying to check
		if (branch == nullptr || branch == this)
		{
			//Continue to the next iteration
			continue;
		}
		//If the branches intersect
		else if (Intersects(*branch))
		{
			//Return true
			return true;
		}
	}
	//Return false if there are no collisions
	return false;
}

//Gets the length of the first section of the branch path
int Branch::GetStartLength() const
{
	return startLength;
}

//Gets the direction of the first section
Direction Branch::GetStartDirection() const
{
	return direction;
}

//Gets the length of the middle section of the branch path
int Branch::GetMidLength() const
{
	return midLength;
}

//Gets the direction of the middle section
Direction Branch::GetMidDirection() const
{
	return midSectionDirection;
}

//Gets the length of the last section of the branch path
int Branch::GetEndLength() const
{
	return endLength;
}

//Gets the direction of the last section
Direction Branch::GetEndDirection() const
{
	return direction;
}
