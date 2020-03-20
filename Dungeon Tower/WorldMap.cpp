#include "WorldMap.h"
#include "Common.h"
#include "Branch.h"
#include "Math.h"


using namespace sf;
using namespace std;

namespace
{
	template<typename type>
	Vector2<type> SmallestParts(Vector2<type> A, Vector2<type> B)
	{
		return Vector2<type>(A.x < B.x ? A.x : B.x, A.y < B.y ? A.y : B.y);
	}

	template<typename type>
	Vector2<type> GreatestParts(Vector2<type> A, Vector2<type> B)
	{
		return Vector2<type>(A.x > B.x ? A.x : B.x, A.y > B.y ? A.y : B.y);
	}
}

void WorldMap::Generate(int level)
{
	this->level = level;

	progress = 0.0f;

	int roomsToGenerate = RandomNumber(2 + level, 6 + level);

	Vector2<int> dimensions = Vector2<int>(RandomNumber(Room::MinRoomWidth, Room::MaxRoomWidth),RandomNumber(Room::MinRoomHeight,Room::MaxRoomHeight));

	TopRoom = make_shared<Room>(Vector2<int>(0,0),dimensions);

	for (int i = 0; i < roomsToGenerate; i++)
	{
		TopRoom->AddRoomToHierarchy(RandomNumber(0,8));

		progress = Math::Lerp(0.0f, 0.9f, static_cast<float>(i) / static_cast<float>(roomsToGenerate - 1));
	}

	progress = 0.9f;

	Flatten();

	progress = 1.0f;

	EnableRendering();
}

void WorldMap::Flatten()
{
	float previousProgress = progress;

	Vector2<int> BottomLeft = { 0,0 };
	Vector2<int> TopRight = { 0,0 };

	for (Room* room : TopRoom->GetAllConnectedRooms())
	{
		Rect<int> roomRect = room->GetRect();

		BottomLeft = SmallestParts(BottomLeft, { roomRect.left,roomRect.top - roomRect.height });
		TopRight = GreatestParts(TopRight, { roomRect.left + roomRect.width,roomRect.top });
	}

	Vector2<int> Dimensions = TopRight - BottomLeft + Vector2<int>(1, 1);

	tiles = decltype(tiles)(Dimensions, nullptr);

	auto rooms = TopRoom->GetAllConnectedRooms();
	auto branches = TopRoom->GetAllConnectedBranches();

	int loopCount = rooms.size() + branches.size();
	int loopCounter = 0;

	tileSize = Vector2u(0, 0);

	for (Room* room : rooms)
	{
		Rect<int> roomRect = room->GetRect();

		Vector2i RoomBottomLeft = Vector2<int>(roomRect.left, roomRect.top - roomRect.height);

		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				Vector2<int> relativePos = Vector2<int>(x, y);
				Vector2<int> layerPos = relativePos + RoomBottomLeft - BottomLeft;

				auto& tile = room->GetTile(relativePos);

				tileSize = tile->GetSprite().getTexture()->getSize();

				Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * tileSize.x), static_cast<float>(layerPos.y * tileSize.y));

				tile->GetSprite().setPosition(tilePosition);

				tiles[layerPos] = tile;

			}
		}

		if (room == TopRoom.get())
		{
			SpawnPoint = (room->GetDimensions() / 2) + Vector2<int>(roomRect.left, roomRect.top - roomRect.height) - BottomLeft;
			SpawnPoint = Vector2i(SpawnPoint.x * tileSize.x, SpawnPoint.y * tileSize.y);
		}

		for (auto& enemySpawnPoint : room->GetEnemySpawnPoints())
		{
			enemySpawnPoints.push_back(Vector2f((enemySpawnPoint.x + RoomBottomLeft.x - BottomLeft.x) * tileSize.x, (enemySpawnPoint.y + RoomBottomLeft.y - BottomLeft.y) * tileSize.y));
		}

		progress = Math::Lerp(previousProgress, 1.0f, static_cast<float>(++loopCounter) / static_cast<float>(loopCount));
	}

	for (Branch* branch : branches)
	{
		for (auto tile : branch->GetTiles())
		{
			Vector2<int> layerPos = Vector2<int>(tile->GetSprite().getPosition()) - BottomLeft;

			auto textureSize = tile->GetSprite().getTexture()->getSize();

			Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * textureSize.x), static_cast<float>(layerPos.y * textureSize.y));

			tile->GetSprite().setPosition(tilePosition);

			tiles[layerPos] = tile;
		}
		progress = Math::Lerp(previousProgress, 1.0f, static_cast<float>(++loopCounter) / static_cast<float>(loopCount));
	}

	int randomRoomNumber = RandomNumber(1, rooms.size());

	Room* doorRoom = rooms[randomRoomNumber];

	Rect<int> doorRoomRect = doorRoom->GetRect();

	Vector2i doorRoomBottomLeft = Vector2<int>(doorRoomRect.left, doorRoomRect.top - doorRoomRect.height);

	int doorX = RandomNumber(1, doorRoom->GetWidth() - 2);
	int doorY = RandomNumber(1, doorRoom->GetHeight() - 2);

	doorLocation = {(doorX + doorRoomBottomLeft.x + BottomLeft.x) * tileSize.x, (doorY + doorRoomBottomLeft.y + BottomLeft.y) * tileSize.y};
}

WorldMap::WorldMap() :
	progress(0.0f)
{

}

const atomic<float>& WorldMap::GetProgress() const
{
	return progress;
}

void WorldMap::GenerateAsync(int level)
{
	if (thread == nullptr)
	{
		thread = std::make_unique<std::thread>(&WorldMap::Generate, this, level);

		thread->detach();
	}
}

int WorldMap::GetWidth() const
{
	return tiles.GetWidth();
}

int WorldMap::GetHeight() const
{
	return tiles.GetHeight();
}

sf::Vector2u WorldMap::GetTileSize() const
{
	return tileSize;
}

Vector2<int> WorldMap::GetSpawnPoint() const
{
	return SpawnPoint;
}

const std::vector<sf::Vector2f>& WorldMap::GetEnemySpawnPoints() const
{
	return enemySpawnPoints;
}

std::vector<sf::Vector2f>& WorldMap::GetEnemySpawnPoints()
{
	return enemySpawnPoints;
}

sf::Vector2f WorldMap::GetDoorLocation() const
{
	return doorLocation;
}

void WorldMap::Render(RenderWindow& window)
{
	auto view = window.getView();

	auto viewRect = Rect<int>((Vector2i(view.getCenter())), Vector2i(view.getSize()));


	Vector2i textureSize = static_cast<Vector2i>(Common::Textures::centerPiece1.GetTexture().getSize());

	auto mapView = Rect<int>(viewRect.left / textureSize.x, viewRect.top / textureSize.y, viewRect.width / textureSize.x, viewRect.height / textureSize.y);

	for (int x = mapView.left - 2; x < (mapView.left + mapView.width) + 2; x++)
	{
		for (int y = (mapView.top - mapView.height) - 2; y < mapView.top + 2; y++)
		{
			int newX = x - (mapView.width / 2);
			int newY = y + (mapView.height / 2);
			if (newX < 0 || newX >= tiles.GetWidth() || newY < 0 || newY >= tiles.GetHeight())
			{
				continue;
			}
			else
			{
				auto tile = tiles[{newX, newY}];
				if (tile != nullptr)
				{
					window.draw(tiles[{newX, newY}]->GetSprite());
				}
			}
		}
	}
}

bool WorldMap::HasTile(int x, int y, BackgroundTile& output) const
{
	if (HasTile(x,y))
	{
		output = *tiles.Get(x, y);
		return true;
	}
	else
	{
		return false;
	}
}

bool WorldMap::HasTile(int x, int y) const
{
	return tiles.WithinBounds(x, y) && tiles.Get(x, y) != nullptr;
}

bool WorldMap::GetLevel() const
{
	return level;
}

bool WorldMap::IsWithinBounds(int x, int y) const
{
	return tiles.WithinBounds(x, y);
}

BackgroundTile* WorldMap::GetTile(int x, int y) const
{
	if (HasTile(x,y))
	{
		return tiles.Get(x, y).get();
	}
	else
	{
		return nullptr;
	}
}

BackgroundTile* WorldMap::operator[](Vector2<int> position) const
{
	return GetTile(position.x, position.y);
}

Array2D<BackgroundTile*> WorldMap::GetTilesWithinRect(sf::FloatRect rect) const
{
	Rect<int> area = Rect<int>(floorf(rect.left) / tileSize.x ,ceilf(rect.top) / tileSize.y,ceilf(rect.width) / tileSize.x,ceilf(rect.height) / tileSize.y );

	Array2D<BackgroundTile*> grid{area.width,area.height,nullptr};

	grid.SetOffset({area.left,(area.top - area.height)});

	for (int x = area.left; x < area.left + area.width; x++)
	{
		for (int y = area.top - area.height; y < area.top; y++)
		{
			grid.Set(GetTile(x,y),x,y);
		}
	}

	grid.SetOffset({ area.left * static_cast<int>(tileSize.x),(area.top - area.height)* static_cast<int>(tileSize.y) });

	return grid;
}
