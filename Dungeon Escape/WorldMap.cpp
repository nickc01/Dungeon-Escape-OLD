#include "WorldMap.h" //Contains the WorldMap class
#include "Common.h" //Contains many common game functions and variables
#include "Branch.h" //Contains the Branch class for creating paths to connect rooms
#include "Math.h" //Contains many commonly used math functions


using namespace sf; //Prevents me from having to type sf everywhere
using namespace std; //Prevents me from having to type std everywhere

namespace
{
	//Takes two vectors and returns their lowest components
	template<typename type>
	Vector2<type> SmallestParts(Vector2<type> A, Vector2<type> B)
	{
		return Vector2<type>(A.x < B.x ? A.x : B.x, A.y < B.y ? A.y : B.y);
	}

	//Takes two vectors and returns their hightest components
	template<typename type>
	Vector2<type> GreatestParts(Vector2<type> A, Vector2<type> B)
	{
		return Vector2<type>(A.x > B.x ? A.x : B.x, A.y > B.y ? A.y : B.y);
	}
}

//Generate the room hierarchy
void WorldMap::Generate(int level)
{
	//Store the difficulty level
	this->level = level;

	//Reset the generation progress
	progress = 0.0f;

	//Calculate how many rooms we are going to generate
	int roomsToGenerate = Common::RandomNumber(2 + level, 6 + level);

	//Create random dimensions for the top-most room
	Vector2<int> dimensions = Vector2<int>(Common::RandomNumber(Room::MinRoomWidth, Room::MaxRoomWidth),Common::RandomNumber(Room::MinRoomHeight,Room::MaxRoomHeight));

	//Create the top-most room
	TopRoom = make_shared<Room>(Vector2<int>(0,0),dimensions);

	//Loop over all the rooms that we are generating
	for (int i = 0; i < roomsToGenerate; i++)
	{
		//Add a new room to the room hierarchy
		TopRoom->AddRoomToHierarchy(Common::RandomNumber(0,15));

		//Increase the generation progress
		progress = Math::Lerp(0.0f, 0.9f, static_cast<float>(i) / static_cast<float>(roomsToGenerate - 1));
	}

	//Set the progress to 90% complete
	progress = 0.9f;

	//Flatten the room and branch hierarchy to a 2D grid
	Flatten();

	//Set the progress to 100% complete
	progress = 1.0f;

	//Start rendering the world map
	EnableRendering();
}

//Takes the room and branch hierachy, and flattens it into a 2D grid of tiles
void WorldMap::Flatten()
{
	//Stores the previous progress before flattening
	float previousProgress = progress;

	//Stores the bottom-left corner of the map
	Vector2<int> BottomLeft = { 0,0 };

	//Stores the top-right corner of the map
	Vector2<int> TopRight = { 0,0 };

	//Loop over all the rooms in the room hierarchy
	for (Room* room : TopRoom->GetAllRoomsInHierarchy())
	{
		//Get the room's rect bounds
		Rect<int> roomRect = room->GetRect();

		//If it's bottom left corner is lower than the stored one, then update it
		BottomLeft = SmallestParts(BottomLeft, { roomRect.left,roomRect.top - roomRect.height });
		//If it's top right corner is higher than the stored one, then update it
		TopRight = GreatestParts(TopRight, { roomRect.left + roomRect.width,roomRect.top });
	}

	//Get the dimensions of the world map
	Vector2<int> Dimensions = TopRight - BottomLeft + Vector2<int>(1, 1);

	//Create the 2D array with the set dimensions
	tiles = decltype(tiles)(Dimensions, nullptr);

	//Get all the rooms in the room hierarch
	auto rooms = TopRoom->GetAllRoomsInHierarchy();

	//Get all the branches in the room hierarchy
	auto branches = TopRoom->GetAllBranchesInHierarchy();

	//The amount of rooms and branches to iterated over. Used to increment the progress
	int loopCount = rooms.size() + branches.size();
	int loopCounter = 0; //Keeps track of how many rooms and branches have currently been iterated over

	//Stores the size of each tile
	tileSize = Vector2u(0, 0);

	//Loop over all the rooms in the hierarchy
	for (Room* room : rooms)
	{
		//Get the rect of the room
		Rect<int> roomRect = room->GetRect();

		//Get the bottomleft corner of the room
		Vector2i RoomBottomLeft = Vector2<int>(roomRect.left, roomRect.top - roomRect.height);

		//Loop over all the tiles in the room
		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				//Get the tile position relative to the bottom left corner of the room
				Vector2<int> relativePos = Vector2<int>(x, y);

				//Convert the tile coordinates to map coordinates
				Vector2<int> layerPos = relativePos + RoomBottomLeft - BottomLeft;

				//Get the tile at the specified position
				auto& tile = room->GetTile(relativePos);

				//Get and store the size of the tile
				tileSize = tile->GetSprite().getTexture()->getSize();

				//Get the world space position of the tile
				Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * tileSize.x), static_cast<float>(layerPos.y * tileSize.y));

				//Set the tile's world space position
				tile->GetSprite().setPosition(tilePosition);

				//Add the tile to the tile grid
				tiles[layerPos] = tile;

			}
		}

		//If this room is the same as the top room
		if (room == TopRoom.get())
		{
			//Set the player's spawnpoint to be at the center of the room
			SpawnPoint = (room->GetDimensions() / 2) + Vector2<int>(roomRect.left, roomRect.top - roomRect.height) - BottomLeft;
			SpawnPoint = Vector2i(SpawnPoint.x * tileSize.x, SpawnPoint.y * tileSize.y);
		}

		//Loop over all the potential enemy spawnpoints in the room
		for (auto& enemySpawnPoint : room->GetEnemySpawnPoints())
		{
			//Convert the spawnpoint to world coordinates and add it to the list of spawnpoints
			enemySpawnPoints.push_back(Vector2f((enemySpawnPoint.x + RoomBottomLeft.x - BottomLeft.x) * tileSize.x, (enemySpawnPoint.y + RoomBottomLeft.y - BottomLeft.y) * tileSize.y));
		}

		//Increase the generation progress
		progress = Math::Lerp(previousProgress, 1.0f, static_cast<float>(++loopCounter) / static_cast<float>(loopCount));
	}

	//Loop over all the branches in the room hierarchy
	for (Branch* branch : branches)
	{
		//Get all the tiles that make up the branch
		for (auto tile : branch->GetTiles())
		{
			//Get the map map coordinates of the tile
			Vector2<int> layerPos = Vector2<int>(tile->GetSprite().getPosition()) - BottomLeft;

			//Get the size of the tile texture
			auto textureSize = tile->GetSprite().getTexture()->getSize();

			//Get the world space coordinates of the tile
			Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * textureSize.x), static_cast<float>(layerPos.y * textureSize.y));

			//Set the world space position of the tile
			tile->GetSprite().setPosition(tilePosition);

			//Add the tile to the 2D grid
			tiles[layerPos] = tile;
		}
		//Increase the progress
		progress = Math::Lerp(previousProgress, 1.0f, static_cast<float>(++loopCounter) / static_cast<float>(loopCount));
	}

	//Pick a random room to hold the exit door
	int randomRoomNumber = Common::RandomNumber(1, rooms.size());

	//Get the room at the random index
	Room* doorRoom = rooms[randomRoomNumber];

	//Get the rect of the door room
	Rect<int> doorRoomRect = doorRoom->GetRect();

	//Get the bottom left corner of the door room
	Vector2i doorRoomBottomLeft = Vector2<int>(doorRoomRect.left, doorRoomRect.top - doorRoomRect.height);

	//Get random x and y coordinates to place the door at
	int doorX = Common::RandomNumber(1, doorRoom->GetWidth() - 2);
	int doorY = Common::RandomNumber(1, doorRoom->GetHeight() - 2);

	//Conver the coordinates to world space and set that as the door location
	doorLocation = Vector2f((doorX + doorRoomBottomLeft.x - BottomLeft.x) * static_cast<float>(tileSize.x), (doorY + doorRoomBottomLeft.y - BottomLeft.y) * static_cast<float>(tileSize.y));
}

//Constructs an empty world map
WorldMap::WorldMap() :
	progress(0.0f)
{

}

//Retrieves the progress of the world generation
const atomic<float>& WorldMap::GetProgress() const
{
	return progress;
}

//Generates the world map on a seperate thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1.0f, the map is done generating
void WorldMap::GenerateAsync(int level)
{
	//If the thread is not already running
	if (thread == nullptr)
	{
		//Create the thread and start the world generation
		thread = std::make_unique<std::thread>(&WorldMap::Generate, this, level);

		//Detach the thread so there is no need to join with it
		thread->detach();
	}
}

//Gets the width of the world map
int WorldMap::GetWidth() const
{
	return tiles.GetWidth();
}

//Gets the height of the world map
int WorldMap::GetHeight() const
{
	return tiles.GetHeight();
}

//Gets the size of each tile in the map
sf::Vector2u WorldMap::GetTileSize() const
{
	return tileSize;
}

//Gets the spawnpoint of the player in the world map
Vector2<int> WorldMap::GetSpawnPoint() const
{
	return SpawnPoint;
}

//Gets all the enemy spawnpoints in the map
const std::vector<sf::Vector2f>& WorldMap::GetEnemySpawnPoints() const
{
	return enemySpawnPoints;
}

//Gets all the enemy spawnpoints in the map
std::vector<sf::Vector2f>& WorldMap::GetEnemySpawnPoints()
{
	return enemySpawnPoints;
}

//Gets the location of the exit door
sf::Vector2f WorldMap::GetDoorLocation() const
{
	return doorLocation;
}

//Draws the world map to the screen
void WorldMap::Render(RenderWindow& window)
{
	//Get the currently set render view of the window
	auto view = window.getView();

	//Get the center of the view
	auto viewCenter = view.getCenter();

	//Get the size of the view
	auto viewSize = view.getSize();
	
	//Get the rect bounds that encompasses the entire viewing area
	auto viewRect = Rect<int>(viewCenter.x - (viewSize.x / 2.0f), viewCenter.y + (viewSize.y / 2.0f), viewSize.x, viewSize.y);


	//Get the size of the textures used in the map
	Vector2i textureSize = static_cast<Vector2i>(Common::Textures::centerPiece1.GetTexture().getSize());

	//Convert the viewing rect into map coordinates
	auto mapView = Rect<int>(viewRect.left / textureSize.x, viewRect.top / textureSize.y, viewRect.width / textureSize.x, viewRect.height / textureSize.y);

	//Loop over the entire map viewing rect, with an offset of 3 applied to prevent artifacts from appearing on the sides of the screen
	for (int x = mapView.left - 3; x < (mapView.left + mapView.width) + 3; x++)
	{
		for (int y = (mapView.top - mapView.height) - 3; y < mapView.top + 3; y++)
		{
			//If the coordinates are not within the boundaries of the map
			if (x < 0 || x >= tiles.GetWidth() || y < 0 || y >= tiles.GetHeight())
			{
				//Continue to the next tile
				continue;
			}
			//if the coordinates are within the map boundaries
			else
			{
				//Get the tiles at that location
				auto tile = tiles[{x, y}];
				//If there is a tile here
				if (tile != nullptr)
				{
					//Draw it to the screen
					window.draw(tiles[{x, y}]->GetSprite());
				}
			}
		}
	}
}

//Tests to see if a tile exists at the specified location. If true, then the output reference will be modified
bool WorldMap::HasTile(int x, int y, BackgroundTile& output) const
{
	//If there is a tile at the specified coordinates
	if (HasTile(x,y))
	{
		//Update the reference
		output = *tiles.Get(x, y);
		return true;
	}
	//If there is no tile here
	else
	{
		return false;
	}
}

//Tests to see if a tile exists at the specified location.
bool WorldMap::HasTile(int x, int y) const
{
	//If the tile is within the boundaries of the map and a tile exists at the specified location, return true, and false otherwise
	return tiles.WithinBounds(x, y) && tiles.Get(x, y) != nullptr;
}

//Get the difficulty level of the world map
bool WorldMap::GetLevel() const
{
	return level;
}

//Tests if the specified coordinate is within the boundaries of the map
bool WorldMap::IsWithinBounds(int x, int y) const
{
	return tiles.WithinBounds(x, y);
}

//Gets a tile at the specified coordinate. If no tile exists there, null will be returned
BackgroundTile* WorldMap::GetTile(int x, int y) const
{
	//If there is a tile here
	if (HasTile(x,y))
	{
		//Get the tile
		return tiles.Get(x, y).get();
	}
	//If no there is here
	else
	{
		//Return null
		return nullptr;
	}
}

//Gets a tile at the specified coordinate. If no tile exists there, null will be returned
BackgroundTile* WorldMap::operator[](Vector2<int> position) const
{
	//Get the tile at the position
	return GetTile(position.x, position.y);
}

//Retrieves all the tiles that are wintin the specified rectangle
Array2D<BackgroundTile*> WorldMap::GetTilesWithinRect(sf::FloatRect rect) const
{
	//Convert the rect to map coordinates
	Rect<int> area = Rect<int>(floorf(rect.left) / tileSize.x ,ceilf(rect.top) / tileSize.y,ceilf(rect.width) / tileSize.x,ceilf(rect.height) / tileSize.y );

	//Create the tile grid
	Array2D<BackgroundTile*> grid{area.width,area.height,nullptr};

	//Set the offset to be the bottom-left corner of the map rect
	grid.SetOffset({area.left,(area.top - area.height)});

	//Loop over the entire map rectangle
	for (int x = area.left; x < area.left + area.width; x++)
	{
		for (int y = area.top - area.height; y < area.top; y++)
		{
			//Get the tile at the specified position and add it to the 2D grid
			grid.Set(GetTile(x,y),x,y);
		}
	}

	//Convert the offset to world coordinates
	grid.SetOffset({ area.left * static_cast<int>(tileSize.x),(area.top - area.height)* static_cast<int>(tileSize.y) });

	//Return the grid
	return grid;
}

//The destructor
WorldMap::~WorldMap()
{
	//Disable rendering
	EnableRendering(false);
}
