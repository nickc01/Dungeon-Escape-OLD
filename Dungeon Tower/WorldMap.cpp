#include "WorldMap.h"
#include "Common.h"
#include "Branch.h"


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
	progress = 0.0f;

	int roomsToGenerate = RandomNumber(2 + level, 6 + level);

	Vector2<int> dimensions = Vector2<int>(RandomNumber(10, 24),RandomNumber(10,24));

	TopRoom = make_shared<Room>(Vector2<int>(0,0),dimensions);

	for (int i = 0; i < roomsToGenerate; i++)
	{
		TopRoom->AddRoomToHierarchy();
	}

	Flatten();

	progress = 1.0f;
}

void WorldMap::Flatten()
{
	Vector2<int> BottomLeft = {0,0};
	Vector2<int> TopRight = {0,0};

	for (Room* room : TopRoom->GetAllConnectedRooms())
	{
		Rect<int> roomRect = room->GetRect();

		BottomLeft = SmallestParts(BottomLeft, {roomRect.left,roomRect.top - roomRect.height});
		TopRight = GreatestParts(TopRight, {roomRect.left + roomRect.width,roomRect.top});
	}

	Vector2<int> Dimensions = TopRight - BottomLeft + Vector2<int>(1,1);

	tiles = decltype(tiles)(Dimensions, nullptr);

	auto rooms = TopRoom->GetAllConnectedRooms();


	for (Room* room : rooms)
	{
		Rect<int> roomRect = room->GetRect();

		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				Vector2<int> relativePos = Vector2<int>(x, y);
				Vector2<int> layerPos = relativePos + Vector2<int>(roomRect.left, roomRect.top - roomRect.height) - BottomLeft;

				auto& tile = room->GetTile(relativePos);

				auto textureSize = tile->GetSprite().getTexture()->getSize();

				Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * textureSize.x), static_cast<float>(layerPos.y * textureSize.y));

				tile->GetSprite().setPosition(tilePosition);

				tiles[layerPos] = tile;

			}
		}

		if (room == TopRoom.get())
		{
			SpawnPoint = (room->GetDimensions() / 2) + Vector2<int>(roomRect.left, roomRect.top - roomRect.height) - BottomLeft;
		}
	}

	for (Branch* branch : TopRoom->GetAllConnectedBranches())
	{
		for (auto tile : branch->GetTiles())
		{
			Vector2<int> layerPos = Vector2<int>(tile->GetSprite().getPosition()) - BottomLeft;

			auto textureSize = tile->GetSprite().getTexture()->getSize();

			Vector2<float> tilePosition = Vector2<float>(static_cast<float>(layerPos.x * textureSize.x), static_cast<float>(layerPos.y * textureSize.y));

			tile->GetSprite().setPosition(tilePosition);

			tiles[layerPos] = tile;
		}
	}
}

WorldMap::WorldMap() :
	progress(0.0f)
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

int WorldMap::GetWidth() const
{
	return tiles.GetWidth();
}

int WorldMap::GetHeight() const
{
	return tiles.GetHeight();
}

Vector2<int> WorldMap::GetSpawnPoint() const
{
	return SpawnPoint;
}

void WorldMap::Render(RenderWindow& window) const
{
	auto view = window.getView();

	auto viewRect = Rect<int>((Vector2i(view.getCenter())), Vector2i(view.getSize()));


	Vector2i textureSize = static_cast<Vector2i>(Common::Textures::blankTexture.GetTexture().getSize());

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
