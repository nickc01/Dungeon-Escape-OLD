#include "WorldMap.h"
#include "Common.h"

using namespace std;

void WorldMap::Generate(int level)
{
	progress = 0.0f;

	int roomsToGenerate = RandomNumber(2 + level, 6 + level);

	Vector2 dimensions = Vector2(RandomNumber(20, 48),RandomNumber(10,24));

	TopRoom = make_shared<Room>(Vector2(0,0),dimensions);

	for (int i = 0; i < roomsToGenerate; i++)
	{
		TopRoom->AddRoomToHierarchy();
	}

	progress = 1.0f;
}

WorldMap::WorldMap()
{
	
}

float WorldMap::GetProgress()
{
	return progress;
}

void WorldMap::GenerateAsync(int level)
{
	if (thread == nullptr)
	{
		thread = std::make_unique<std::thread>(&WorldMap::Generate, this, level);
	}
}

shared_ptr<Room> WorldMap::GetFirstRoom()
{
	return TopRoom;
}
