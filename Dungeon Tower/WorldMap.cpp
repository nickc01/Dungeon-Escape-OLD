#include "WorldMap.h"
#include "Common.h"
#include "Branch.h"
#include "Renderer.h"

using namespace std;

void WorldMap::Generate(int level)
{
	progress = 0.0f;

	int roomsToGenerate = RandomNumber(2 + level, 6 + level);

	Vector2I dimensions = Vector2I(RandomNumber(10, 24),RandomNumber(10,24));

	TopRoom = make_shared<Room>(Vector2I(0,0),dimensions);

	for (int i = 0; i < roomsToGenerate; i++)
	{
		TopRoom->AddRoomToHierarchy();
	}

	Flatten();

	progress = 1.0f;
}

void WorldMap::Flatten()
{
	Vector2I BottomLeft = {0,0};
	Vector2I TopRight = {0,0};

	for (Room* room : TopRoom->GetAllConnectedRooms())
	{
		RectI roomRect = room->GetRect();

		BottomLeft = VectorMath::SmallestParts(BottomLeft, roomRect.BottomLeft());
		TopRight = VectorMath::GreatestParts(TopRight, roomRect.TopRight());
	}

	Vector2I Dimensions = TopRight - BottomLeft + Vector2I(1,1);

	backgroundLayer = decltype(backgroundLayer)(Dimensions, nullptr);

	auto rooms = TopRoom->GetAllConnectedRooms();


	for (Room* room : rooms)
	{
		RectI roomRect = room->GetRect();

		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				Vector2I relativePos = Vector2I(x, y);
				Vector2I layerPos = relativePos + roomRect.BottomLeft() - BottomLeft;

				auto& tile = room->GetTile(relativePos);

				auto textureSize = tile->GetSprite().getTexture()->getSize();

				Vector2 tilePosition = Vector2(layerPos.x * textureSize.x,layerPos.y * textureSize.y);

				tile->GetSprite().setPosition(tilePosition);
				//tile->SetP(tilePosition);

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
			Vector2I layerPos = Vector2I(tile->GetSprite().getPosition()) - BottomLeft;

			auto textureSize = tile->GetSprite().getTexture()->getSize();

			Vector2 tilePosition = Vector2(layerPos.x * textureSize.x, layerPos.y * textureSize.y);

			tile->GetSprite().setPosition(tilePosition);

			//Common::window.draw(tile->GetSprite());
			//Common::window.display();
			//tile->SetPosition(tile->GetPosition() * 16.0f);

			backgroundLayer[layerPos] = tile;
		}
	}
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

/*Vector2I WorldMap::GetBottomLeft() const
{
	return BottomLeft;
}

Vector2I WorldMap::GetTopRight() const
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

Vector2I WorldMap::GetSpawnPoint() const
{
	return SpawnPoint;
}

void WorldMap::Render(sf::RenderWindow& window) const
{
	auto view = window.getView();

	auto viewRect = sf::Rect<int>((sf::Vector2i(view.getCenter())), sf::Vector2i(view.getSize()));

	auto bottomLeft = Vector2I(viewRect.left, viewRect.top - viewRect.height);
	auto topRight = Vector2I(viewRect.left + viewRect.width, viewRect.top);

	auto worldBL = bottomLeft / (int)Common::Textures::blankTexture.GetTexture().getSize().x;
	auto worldTR = topRight / (int)Common::Textures::blankTexture.GetTexture().getSize().y;

	auto worldView = sf::Rect<float>(worldBL.x, worldTR.y, worldTR.x - worldBL.x, worldTR.y - worldBL.y);


	auto worldViewClamped = sf::Rect<int>(floorf(worldView.left), ceilf(worldView.top), ceilf(worldView.width), ceilf(worldView.height));


	for (int x = worldViewClamped.left - 2; x < (worldViewClamped.left + worldViewClamped.width) + 2; x++)
	{
		for (int y = (worldViewClamped.top - worldViewClamped.height) - 2; y < worldViewClamped.top + 2; y++)
		{
			int newX = x - (worldViewClamped.width / 2);
			int newY = y + (worldViewClamped.height / 2);
			if (newX < 0 || newX >= backgroundLayer.GetWidth() || newY < 0 || newY >= backgroundLayer.GetHeight())
			{

			}
			else
			{
				auto tile = backgroundLayer[{newX, newY}];
				if (tile != nullptr)
				{
					window.draw(backgroundLayer[{newX, newY}]->GetSprite());
				}
			}
		}
	}
}

/*Array2D<std::shared_ptr<BackgroundTile>>& WorldMap::GetBackgroundLayer()
{
	return backgroundLayer;
}*/
