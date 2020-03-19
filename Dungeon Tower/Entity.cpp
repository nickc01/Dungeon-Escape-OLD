#include "Entity.h"
#include "Common.h"
#include "Math.h"

using namespace std;
using namespace sf;

namespace
{
	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Sprite sprite, Vector2u tileSize, Vector2f offset);
}

void Entity::Render(sf::RenderWindow& window) const
{
	if (sprite != nullptr)
	{
		window.draw(*sprite);
	}
}

Array2D<BackgroundTile*> Entity::GetTilesAroundEntity() const
{
	if (sprite == nullptr)
	{
		return Array2D<BackgroundTile*>(0, 0, nullptr);
	}

	auto position = sprite->getPosition();
	auto entityRect = sprite->getGlobalBounds();

	auto width = entityRect.width;
	auto height = entityRect.height;

	auto right = entityRect.left + width;
	auto bottom = entityRect.top - height;
	auto left = entityRect.left;
	auto top = entityRect.top;

	top += (height * 5);
	bottom -= (height * 5);
	right += (width * 5);
	left -= (width * 5);

	return map.GetTilesWithinRect(Rect<float>(left, top, right - left, top - bottom));
}

Entity::Entity(const WorldMap& map, bool renderingEnabled, bool enableUpdating) :
	Entity(map,nullptr,renderingEnabled,enableUpdating)
{

}

Entity::Entity(const WorldMap& map, sf::Sprite* sprite, bool renderingEnabled, bool enableUpdating) :
	Renderable(renderingEnabled),
	UpdateReceiver(enableUpdating),
	map(map),
	renderingEnabled(renderingEnabled),
	sprite(sprite)
{
}

sf::Sprite* Entity::GetSprite()
{
	return sprite;
}

const sf::Sprite* Entity::GetSprite() const
{
	return sprite;
}

void Entity::SetSprite(sf::Sprite* newSprite)
{
	if (sprite == newSprite)
	{
		return;
	}
	if (sprite == nullptr)
	{
		sprite = newSprite;
	}
	else
	{
		auto previousPosition = sprite->getPosition();
		auto previousScale = sprite->getScale();
		auto previousOrigin = sprite->getOrigin();
		auto previousRotation = sprite->getRotation();

		sprite = newSprite;

		sprite->setPosition(previousPosition);
		sprite->setScale(previousScale);
		sprite->setOrigin(previousOrigin);
		sprite->setRotation(previousRotation);
	}
}

bool Entity::GetCollisionMode()
{
	return renderingEnabled;
}

void Entity::SetCollisionMode(bool enabled)
{
	renderingEnabled = enabled;
}

void Entity::Move(float x, float y)
{
	Move({x,y});
}

void Entity::Move(sf::Vector2f direction)
{
	if (sprite == nullptr)
	{
		return;
	}

	if (direction.x != 0 && direction.y != 0)
	{
		auto directionComponents = Math::VectorComponents(direction);
		Move(directionComponents[0]);
		Move(directionComponents[1]);
		return;
	}

	if (true)
	{
		Direction moveDirection = DirectionFromVector(direction);


		auto tiles = GetTilesAroundEntity();

		float distanceToWall = GetDistanceToWall(tiles, moveDirection, *sprite, map.GetTileSize(), { 0.0f,8.0f });
		switch (moveDirection)
		{
		case Direction::Up:
			if (direction.y > distanceToWall)
			{
				direction.y = distanceToWall;
			}
			break;
		case Direction::Right:
			if (direction.x > distanceToWall)
			{
				direction.x = distanceToWall;
			}
			break;
		case Direction::Down:
			if (direction.y < -distanceToWall)
			{
				direction.y = -distanceToWall;
			}
			break;
		case Direction::Left:
			if (direction.x < -distanceToWall)
			{
				direction.x = -distanceToWall;
			}
			break;
		}
	}

	sprite->move(direction);
}

void Entity::Move(Direction direction, int scalar)
{
	Move(VectorInDirection<float>(direction, scalar));
}

void Entity::MoveCameraTo(sf::Vector2f position, float lerpAmount)
{
	sf::View cameraView = Common::MainWindow.getView();

	cameraView.setCenter(Math::VectorLerp(cameraView.getCenter(), position, lerpAmount));

	Common::MainWindow.setView(cameraView);
}




namespace
{
	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Sprite sprite, Vector2u tileSize, Vector2f offset)
	{

		auto bounds = sprite.getGlobalBounds();

		bounds.left -= offset.x;
		bounds.top -= offset.y;

		float nearestTileNumber = 0.0f;

		auto bottomLeft = tiles.GetOffset();

		//tiles.SetOffset({0,0});

		float result = 0.0f;

		switch (direction)
		{
		case Direction::Up:
			nearestTileNumber = INFINITY;
			for (int x = 0; x < tiles.GetWidth(); x++)
			{
				for (int y = 0; y < tiles.GetHeight(); y++)
				{
					auto tile = tiles.GetRelative(x, y);
					if (tile == nullptr || tile->IsCollidable())
					{
						Rect<float> tileLocalBounds = Common::Sprites::centerPiece1.getGlobalBounds();
						Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x), tileLocalBounds.top + bottomLeft.y + (y * tileSize.y), tileLocalBounds.width, tileLocalBounds.height);

						if (tileBounds.left < bounds.left + bounds.width && tileBounds.left + tileBounds.width > bounds.left)
						{
							if (tileBounds.top - tileBounds.height < nearestTileNumber && tileBounds.top > bounds.top)
							{
								nearestTileNumber = tileBounds.top - tileBounds.height;
							}
						}
					}
				}
			}
			result = nearestTileNumber - bounds.top;
			break;
		case Direction::Right:
			nearestTileNumber = INFINITY;
			for (int x = 0; x < tiles.GetWidth(); x++)
			{
				for (int y = 0; y < tiles.GetHeight(); y++)
				{
					auto tile = tiles.GetRelative(x, y);
					if (tile == nullptr || tile->IsCollidable())
					{
						Rect<float> tileLocalBounds = Common::Sprites::centerPiece1.getGlobalBounds();
						Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x), tileLocalBounds.top + bottomLeft.y + (y * tileSize.y), tileLocalBounds.width, tileLocalBounds.height);

						if (tileBounds.top > bounds.top - bounds.height && tileBounds.top - tileBounds.height < bounds.top)
						{
							if (tileBounds.left < nearestTileNumber && tileBounds.left + tileBounds.width > bounds.left + tileBounds.width)
							{
								nearestTileNumber = tileBounds.left;
							}
						}
					}
				}
			}
			result = nearestTileNumber - (bounds.left + bounds.width);
			break;
		case Direction::Down:
			nearestTileNumber = -INFINITY;
			for (int x = 0; x < tiles.GetWidth(); x++)
			{
				for (int y = 0; y < tiles.GetHeight(); y++)
				{
					auto tile = tiles.GetRelative(x, y);
					if (tile == nullptr || tile->IsCollidable())
					{
						Rect<float> tileLocalBounds = Common::Sprites::centerPiece1.getGlobalBounds();
						Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x), tileLocalBounds.top + bottomLeft.y + (y * tileSize.y), tileLocalBounds.width, tileLocalBounds.height);
						//tileBounds.left += (x * tileSize.x);
						//tileBounds.top += (x * tileSize.x);
						//Rect<float> tileBounds = Rect<float>((x * tileSize.x) + previousOffset.x, (y * tileSize.y) + previousOffset.y, tileSize.x, tileSize.y);
						//auto tileBounds = tiles[{x, y}]->GetSprite().getGlobalBounds();

						if (tileBounds.left < bounds.left + bounds.width && tileBounds.left + tileBounds.width > bounds.left)
						{
							if (tileBounds.top > nearestTileNumber&& tileBounds.top - tileBounds.height < bounds.top - bounds.height)
							{
								nearestTileNumber = tileBounds.top;
							}
						}
					}
				}
			}
			result = (bounds.top - bounds.height) - nearestTileNumber;
			break;
		case Direction::Left:
			nearestTileNumber = -INFINITY;
			for (int x = 0; x < tiles.GetWidth(); x++)
			{
				for (int y = 0; y < tiles.GetHeight(); y++)
				{
					auto tile = tiles.GetRelative(x, y);
					if (tile == nullptr || tile->IsCollidable())
					{
						Rect<float> tileLocalBounds = Common::Sprites::centerPiece1.getGlobalBounds();
						Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x), tileLocalBounds.top + bottomLeft.y + (y * tileSize.y), tileLocalBounds.width, tileLocalBounds.height);

						if (tileBounds.top > bounds.top - bounds.height && tileBounds.top - tileBounds.height < bounds.top)
						{
							if (tileBounds.left + tileBounds.width > nearestTileNumber&& tileBounds.left < bounds.left)
							{
								nearestTileNumber = tileBounds.left + tileBounds.width;
							}
						}
					}
				}
			}
			result = bounds.left - nearestTileNumber;
			break;
		}

		//tiles.SetOffset(previousOffset);

		return result;
	}
}