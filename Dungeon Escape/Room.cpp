#include "Room.h" //Contains the Room class that represents individual rooms in the game
#include "Branch.h" //Contains the Branch class for creating paths to connect rooms
#include "Math.h" //Contains many commonly used math functions

using namespace sf; //Prevents me from having to type sf everywhere
using namespace std; //Prevents me from having to type std everywhere

//Finds an room that has a spot avaiable for a new branch to be installed
Room& Room::FindAvailableRoom()
{
	vector<Room*> allRooms{}; //Stores all the rooms that are in the hierarchy
	FindAllRoomsInHierarchy(allRooms); //Get all the room sin the room hierarchy

	vector<Room*> ValidRooms{}; //Stores the rooms that have a branch-off point available

	//Loop over all the avaiable rooms
	for (Room* room : allRooms)
	{
		//Get all the branches that are in the room, including ones that aren't installed yet
		for (auto& branch : room->GetBranches())
		{
			//If this branch is null, then that means that a new branch can fit in it's place
			if (branch != nullptr && (*branch) == nullptr)
			{
				//So add this room to the list of available rooms that can have a new branch installed
				ValidRooms.push_back(room);
				break;
			}
		}
	}

	//Find a random avaiable room
	int randomIndex = Common::RandomNumber(0, ValidRooms.size());
	//Return the randomly selected room
	return *ValidRooms[randomIndex];
}

//Gets all the rooms that are in the room hierarchy
void Room::FindAllRoomsInHierarchy(vector<Room*>& rooms)
{
	//If this room object has not already been added to the list
	if (find(begin(rooms),end(rooms),this) >= end(rooms))
	{
		//Add it to the list
		rooms.push_back(this);
		//Loop over all the branches on this room
		for (auto& branchPtr : GetBranches())
		{
			auto branch = *branchPtr;
			//If there is a branch installed here and it has a destination room
			if (branch != nullptr && branch->GetDestinationRoom() != nullptr)
			{
				//Go to the destination room and call this function recursively to add it and it's children to the list
				branch->GetDestinationRoom()->FindAllRoomsInHierarchy(rooms);
			}
		}
	}
}

//Gets all the branches that are in the room hiearchy
void Room::FindAllBranchesInHierarchy(std::vector<Branch*>& branches)
{
	//Loop over all the branches that are in this room
	for (auto branchPtr : GetBranches())
	{
		auto branch = *branchPtr;
		//If there is a branch installed here
		if (branch != nullptr)
		{
			//If the branch has not been already added to the list
			if (find(begin(branches), end(branches), branch.get()) >= end(branches))
			{
				//Add it to the list
				branches.push_back(branch.get());
				//Get the destination room of the branch
				auto destinationRoom = branch->GetDestinationRoom();

				//If a destination room exists
				if (destinationRoom != nullptr)
				{
					//Recursively add it's branches to the list too
					destinationRoom->FindAllBranchesInHierarchy(branches);
				}
			}
		}
	}
}

//Adds a new enemy spawnpoint to the room
void Room::AddEnemySpawnpoint()
{
	//Repeat until an enemy spawnpoint has been added
	while (true)
	{
		//Get a random spawnpoint
		Vector2i spawnpoint = Vector2i(Common::RandomNumber(1, GetWidth() - 1), Common::RandomNumber(1, GetHeight() - 1));

		//Stores whether there is an enemy spawnpoint added at that location already
		bool added = false;

		//Loop over all the enemy spawnpoints already added
		for (auto& point : enemySpawnPoints)
		{
			//If there is an enemy spawnpoint already added here
			if (point == spawnpoint)
			{
				//The point has been added already
				added = true;
				//Break out
				break;
			}
		}
		//If the enemy has not been already added to the list
		if (!added)
		{
			//Add it to the list
			enemySpawnPoints.push_back(spawnpoint);
			//Break out of the loop
			break;
		}
	}
}

//Contructs a new room
Room::Room(Vector2<int> center, Vector2<int> dimensions, int enemySpawnPoints) :
	Center(center), 
	Dimensions(dimensions),
	backgroundTiles(GetWidth(),GetHeight(),nullptr)
{
	//Get the width and height of the room
	int width = GetWidth();
	int height = GetHeight();

	//Get rect bounds of the room
	auto rect = GetRect();


	//Loop over the entire center part of the room
	for (int x = 1; x <= width - 2; x++)
	{
		for (int y = 1; y <= height - 2; y++)
		{
			//Fill the center with center piece tiles
			backgroundTiles[{x, y}] = BackgroundTile::Create(Common::GetCenterSprite());
		}
	}
	//Loop over the far left and far right sides of the room
	for (int y = 1; y <= height - 2; y++)
	{
		//Add side pieces to the right side and left sides of the room
		backgroundTiles[{width - 1, y}] = BackgroundTile::Create(Common::Sprites::rightPiece);
		backgroundTiles[{0, y}] = BackgroundTile::Create(Common::Sprites::leftPiece);
	}
	//Loop over the far top and far bottom sides of the room
	for (int x = 1; x <= width - 2; x++)
	{
		//Add side pieces to the top and bottom sides of the room
		backgroundTiles[{x, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomPiece);
		backgroundTiles[{x, 0}] = BackgroundTile::Create(Common::Sprites::topPiece);
	}
	//Add the top left corner piece
	backgroundTiles[{0, 0}] = BackgroundTile::Create(Common::Sprites::topLeftPiece);
	//Add the top right corner piece
	backgroundTiles[{width - 1, 0}] = BackgroundTile::Create(Common::Sprites::topRightPiece);
	//Add the bottom left corner piece
	backgroundTiles[{0, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomLeftPiece);
	//Add the bottom right corner piece
	backgroundTiles[{width - 1, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomRightPiece);

	//Loop over all the amount of enemy spawn points to add
	for (int i = 0; i < enemySpawnPoints; i++)
	{
		//Add an enemy spawnpoint
		AddEnemySpawnpoint();
	}
}

//Retrieves the rect bounds of the room
Rect<int> Room::GetRect() const
{
	//Get the left most part of the room
	int left = Center.x - Dimensions.x / 2;
	//Get the top part of the room
	int top = Center.y + Dimensions.y / 2;

	//Return the rect
	return Rect<int>(left, top, Dimensions.x, Dimensions.y);
}

//Gets the center coordinates of the room
Vector2<int> Room::GetCenter() const
{
	return Center;
}

//Sets the center of the room
void Room::SetCenter(Vector2<int> center)
{
	Center = center;
}

//Gets the dimensions of the room
Vector2<int> Room::GetDimensions() const
{
	return Dimensions;
}

//Gets the width of the room
int Room::GetWidth() const
{
	return Dimensions.x;
}

//Gets the height of the room
int Room::GetHeight() const
{
	return Dimensions.y;
}


//Gets the branch that is installed on the specified side of the room
std::shared_ptr<Branch> Room::GetBranch(Direction side)
{
	switch (side)
	{
		//If the side we want is the top side
	case Direction::Up:
		return UpBranch; //Return the top side branch

		//If the side we want is the right side
	case Direction::Right:
		return RightBranch; //Return the right side branch

		//If the side we want is the bottom side
	case Direction::Down:
		return DownBranch; //Return the bottom side branch

		//If the side we want is the left side
	case Direction::Left:
		return LeftBranch; //Return the left side branch

	}
	return nullptr;
}

//Gets all 4 possible branch sides of the room
array<shared_ptr<Branch>*, 4> Room::GetBranches()
{
	//The list of all 4 sides
	auto list = std::array<std::shared_ptr<Branch>*, 4>();

	//Update the list with all 4 side branches
	list[0] = &UpBranch;
	list[1] = &DownBranch;
	list[2] = &LeftBranch;
	list[3] = &RightBranch;

	//Return the list
	return list;
}

//Gets only the branches that are empty
std::vector<std::shared_ptr<Branch>*> Room::GetEmptyBranches()
{
	//The list of all empty branches
	auto list = std::vector<std::shared_ptr<Branch>*>();

	//If the top branch is empty
	if (UpBranch == nullptr)
	{
		//Add the pointer to it to the list
		list.push_back(&UpBranch);
	}
	//If the bottom branch is empty
	if (DownBranch == nullptr)
	{
		//Add the pointer to it to the list
		list.push_back(&DownBranch);
	}
	//If the left branch is empty
	if (LeftBranch == nullptr)
	{
		//Add the pointer to it to the list
		list.push_back(&LeftBranch);
	}
	//If the right branch is empty
	if (RightBranch == nullptr)
	{
		//Add the pointer to it to the list
		list.push_back(&RightBranch);
	}

	//Return the list
	return list;
}

//Adds the branch to a specific side of the room
void Room::SetBranch(Direction direction, std::shared_ptr<Branch> branch)
{
	switch (direction)
	{
		//If the side we want is the top side
	case Direction::Up:
		UpBranch = branch; //Set the branch to the top side
		break;
		//If the side we want is the right side
	case Direction::Right:
		RightBranch = branch; //Set the branch to the right side
		break;
		//If the side we want is the bottom side
	case Direction::Down:
		DownBranch = branch; //Set the branch to the bottom side
		break;
		//If the side we want is the left side
	case Direction::Left:
		LeftBranch = branch; //Set the branch to the left side
		break;
	}
}

//Returns the list of all enemy spawnpoints
const std::vector<sf::Vector2i>& Room::GetEnemySpawnPoints() const
{
	return enemySpawnPoints;
}

//Returns the list of all enemy spawnpoints
std::vector<sf::Vector2i>& Room::GetEnemySpawnPoints()
{
	return enemySpawnPoints;
}

//Checks if the room intersects with a specified background tile
bool Room::Intersects(const BackgroundTile& tile) const
{
	//Get the rect bounds of the room
	auto rectA = Rect<float>(GetRect());
	//Get the rect bounds of the background tile
	auto rectB = tile.GetSprite().getGlobalBounds();
	//Check if they intersect
	return Math::RectsIntersect(rectA, rectB);
}

//Checks to see if this room intersects with another room
bool Room::Intersects(const Room& B) const
{
	//Get the rect bounds of this room
	auto rectA = GetRect();
	//Get the rect bounds of the other room
	auto rectB = B.GetRect();

	//Check if they intersect
	return Math::RectsIntersect(rectA, rectB);
}

//Gets a room's background tile at the specified position
const shared_ptr<BackgroundTile>& Room::GetTile(Vector2<int> position) const
{
	return backgroundTiles[position];
}

//Gets a room's background tile at the specified position
shared_ptr<BackgroundTile>& Room::GetTile(Vector2<int> position)
{
	return backgroundTiles[position];
}

//Gets a room's background tile at the specified position
const std::shared_ptr<BackgroundTile>& Room::operator[](Vector2<int> position) const
{
	return GetTile(position);
}

//Gets a room's background tile at the specified position
std::shared_ptr<BackgroundTile>& Room::operator[](Vector2<int> position)
{
	return GetTile(position);
}

//Checks to see if a newly added room will collide with anything in the room hierarchy
bool Room::CheckForCollision(const Room* addedRoom)
{
	//All the rooms in the hiearachy
	vector<Room*> Rooms{};

	//Get all the rooms in the hierarchy
	FindAllRoomsInHierarchy(Rooms);

	//Loop over all the rooms
	for (auto room : Rooms)
	{
		//If this room is the same as the added one, then skip over it
		if (room == addedRoom)
		{
			continue;
		}
		//If the two rooms intersect with each other
		else if (room->Intersects(*addedRoom))
		{
			//Return true that there is a collision
			return true;
		}
	}

	//All the branches in the hierarchy
	vector<Branch*> Branches{};

	//Get all the branches in the hierarchy
	FindAllBranchesInHierarchy(Branches);

	//Loop over all the branches in the hierarchy
	for (auto branch : Branches)
	{
		//Loop over all the background tiles in the branch
		for (auto tile : branch->GetTiles())
		{
			//Check if the room intersects with any of them
			if (addedRoom->Intersects(*tile))
			{
				//If it does, then return true that there is a collision
				return true;
			}
		}
	}

	//Return false if there are no collisions
	return false;
}

//Adds a new child room to the room hierarchy
void Room::AddRoomToHierarchy(shared_ptr<Room> destinationRoom)
{
	//Loop untill a room has been added
	while (true)
	{

		//Find a room that has an empty branch, so a new one can be added
		Room& sourceRoom = FindAvailableRoom();

		//Find only the empty branches of the room
		auto branches = sourceRoom.GetEmptyBranches();

		//If there are no empty branches
		if (branches.size() == 0)
		{
			//Continue to the next iteration
			continue;
		}

		//Pick a random empty branch to add to
		int randomBranchIndex = Common::RandomNumber(0, branches.size());
		auto branchPtr = branches[randomBranchIndex];
		auto& branch = *branchPtr;


		//Get the rect of the available room
		auto rect = sourceRoom.GetRect();

		//Get the left, right, top, and bottom sides of the available room rect
		int left = rect.left;
		int right = rect.left + rect.width - 1;
		int top = rect.top - 1;
		int bottom = rect.top - rect.height;


		//If we are adding to the top part of the available room
		if (branchPtr == &sourceRoom.UpBranch)
		{
			//Creating a new branch going in the upwards direction
			branch = make_shared<Branch>(Direction::Up);
			//Set the branch's starting point to start somewhere at the top side of the available room
			branch->SetStartPoint({Common::RandomNumber(left + 2,right - 2),top});
		}
		//If we are adding to the bottom part of the available room
		else if (branchPtr == &sourceRoom.DownBranch)
		{
			//Creating a new branch going in the downwards direction
			branch = make_shared<Branch>(Direction::Down);
			//Set the branch's starting point to start somewhere at the bottom side of the available room
			branch->SetStartPoint({ Common::RandomNumber(left + 2,right - 2),bottom });
		}
		//If we are adding to the left part of the available room
		else if (branchPtr == &sourceRoom.LeftBranch)
		{
			//Creating a new branch going in the leftwards direction
			branch = make_shared<Branch>(Direction::Left);
			//Set the branch's starting point to start somewhere at the left side of the available room
			branch->SetStartPoint({ left,Common::RandomNumber(bottom + 2,top - 2) });
		}
		//If we are adding to the right part of the available room
		else if (branchPtr == &sourceRoom.RightBranch)
		{
			//Creating a new branch going in the rightwards direction
			branch = make_shared<Branch>(Direction::Right);
			//Set the branch's starting point to start somewhere at the right side of the available room
			branch->SetStartPoint({ right,Common::RandomNumber(bottom + 2,top - 2) });
		}

		//Set the branche's destination room to the new room we are adding
		branch->SetDestinationRoom(destinationRoom);

		//Get the end point of the branch we are adding
		Vector2<int> EndPoint = branch->GetDestinationPoint();

		//Stores the new center of the destination room
		Vector2<int> DestinationCenter = Vector2<int>(0, 0);

		//Get the rect of the destination room
		auto destRect = destinationRoom->GetRect();
		//Get the center of the destination room
		auto destCenter = destinationRoom->Center;

		switch (branch->GetDirection())
		{
			//If the branch is going up
		case Direction::Up:
			//Set the destination room's center to line up with the end point of the destination
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y + (destCenter.y - (destRect.top - destRect.height)));
			break;
			//If the branch is going right
		case Direction::Right:
			//Set the destination room's center to line up with the end point of the destination
			DestinationCenter = Vector2<int>(EndPoint.x + (destCenter.x - destRect.left), EndPoint.y);
			break;
			//If the branch is going down
		case Direction::Down:
			//Set the destination room's center to line up with the end point of the destination
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y - (destRect.top - destCenter.y) + 1);
			break;
			//If the branch is going left
		case Direction::Left:
			//Set the destination room's center to line up with the end point of the destination
			DestinationCenter = Vector2<int>(EndPoint.x - (destRect.left + destRect.width - destCenter.x) + 1, EndPoint.y);
			break;
		}

		//Set the new center of the destination room
		destinationRoom->SetCenter(DestinationCenter);

		//Get all the rooms in the hierarchy
		auto allRooms = GetAllRoomsInHierarchy();
		//Get all the branches in the hierarchy
		auto allBranches = GetAllBranchesInHierarchy();

		//Check to make sure both the newly added destination room and the newly added branch don't collide with anything
		if (CheckForCollision(destinationRoom.get()))
		{
			//If they do collide with something, remove the branch and the destination room, and try again
			(*branchPtr) = nullptr;
			continue;
		}
		//If no collision occured
		else
		{
			//Return out
			return;
		}
	}
}

//Constructs a new room to be added to the room hierarchy
void Room::AddRoomToHierarchy(int enemySpawnPoints)
{
	//Construct a new room and add it to the room hierarchy
	AddRoomToHierarchy(make_shared<Room>(Vector2<int>(0,0),Vector2<int>(Common::RandomNumber(MinRoomWidth,MaxRoomWidth), Common::RandomNumber(MinRoomHeight, MaxRoomHeight)),enemySpawnPoints));
}

//Gets all the room in the room hierarchy
const std::vector<Room*> Room::GetAllRoomsInHierarchy()
{
	std::vector<Room*> rooms{}; //Stores all the rooms in the hierarchy
	//Retrieve all the rooms in the hierarchy
	FindAllRoomsInHierarchy(rooms);

	return rooms;
}

//Gets all the branches in the room hierarchy
const std::vector<Branch*> Room::GetAllBranchesInHierarchy()
{
	std::vector<Branch*> branches{}; //Stores all the branches in the hierarchy

	//Retrieve all the branches in the hierarchy
	FindAllBranchesInHierarchy(branches);

	return branches;
}
