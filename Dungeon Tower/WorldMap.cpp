#include "WorldMap.h"
#include "Common.h"
#include "Branch.h"
#include "Renderer.h"

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

	Flatten();

	progress = 1.0f;
}

void WorldMap::Flatten()
{
	BottomLeft = {0,0};
	TopRight = {0,0};

	for (Room* room : TopRoom->GetAllConnectedRooms())
	{
		Rect roomRect = room->GetRect();

		BottomLeft = VectorMath::SmallestParts(BottomLeft, roomRect.BottomLeft());
		TopRight = VectorMath::GreatestParts(TopRight, roomRect.TopRight());
	}

	Vector2 Dimensions = TopRight - BottomLeft;

	backgroundLayer = decltype(backgroundLayer)(Dimensions, nullptr);
	//spriteLayer = decltype(spriteLayer)(Dimensions, nullptr);

	for (Room* room : TopRoom->GetAllConnectedRooms())
	{
		Rect roomRect = room->GetRect();

		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				Vector2 roomPos = Vector2(x, y);
				Vector2 layerPos = roomPos + roomRect.BottomLeft() - BottomLeft;

				auto& tile = room->GetTile(roomPos);

				tile->SetPosition(layerPos);

				backgroundLayer[layerPos] = tile;
			}
		}

		if (room == TopRoom.get())
		{
			SpawnPoint = (room->GetDimensions() / 2) + roomRect.BottomLeft() - BottomLeft;
		}
	}

	for (Branch* branch : TopRoom->GetAllConnectedBranches())
	{
		for (auto tile : branch->GetTiles())
		{
			Vector2 layerPos = tile->GetPosition() - BottomLeft;

			tile->SetPosition(layerPos);

			backgroundLayer[layerPos] = tile;
		}
	}

	Renderer::BackgroundTiles = backgroundLayer;
}

WorldMap::WorldMap()
{
	
}

float WorldMap::GetProgress() const
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

/*shared_ptr<Room> WorldMap::GetFirstRoom() const
{
	return TopRoom;
}*/

/*Vector2 WorldMap::GetBottomLeft() const
{
	return BottomLeft;
}

Vector2 WorldMap::GetTopRight() const
{
	return TopRight;
}*/

int WorldMap::GetWidth() const
{
	return backgroundLayer.GetWidth();
}

int WorldMap::GetHeight() const
{
	return backgroundLayer.GetHeight();
}

Vector2 WorldMap::GetSpawnPoint() const
{
	return SpawnPoint;
}

Array2D<std::shared_ptr<BackgroundTile>>& WorldMap::GetBackgroundLayer()
{
	return backgroundLayer;
}

/*std::shared_ptr<BackgroundTile> WorldMap::GetBackgroundTile(int x, int y)
{
	if (x < 0 || x >= backgroundLayer.GetWidth() || y < 0 || y >= backgroundLayer.GetHeight())
	{
		return shared_ptr<BackgroundTile>(nullptr);
	}
	else
	{
		return backgroundLayer.Get(x, y);
	}
}

std::shared_ptr<BackgroundTile> WorldMap::GetBackgroundTile(Vector2 position)
{
	return GetBackgroundTile(get<0>(position), get<1>(position));
}*/
