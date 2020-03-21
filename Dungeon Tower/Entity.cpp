#include "Entity.h"
#include "Common.h"
#include "Math.h"

using namespace std;
using namespace sf;

/*namespace
{
	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Sprite sprite, Vector2u tileSize, Vector2f offset);
}*/

void Entity::Render(sf::RenderWindow& window)
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

Entity::Entity(const WorldMap& map, bool collisionEnabled) :
	Entity(map,nullptr,collisionEnabled)
{

}

Entity::Entity(const WorldMap& map, sf::Sprite* sprite,bool collisionEnabled) :
	map(map),
	collisionEnabled(collisionEnabled),
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
		auto previousColor = sprite->getColor();

		sprite = newSprite;

		if (sprite != nullptr)
		{
			sprite->setPosition(previousPosition);
			sprite->setScale(previousScale);
			sprite->setOrigin(previousOrigin);
			sprite->setRotation(previousRotation);
			sprite->setColor(previousColor);
		}
	}
}

bool Entity::GetCollisionMode()
{
	return collisionEnabled;
}

void Entity::SetCollisionMode(bool enabled)
{
	collisionEnabled = enabled;
}

sf::Rect<float> Entity::GetHitBox() const
{
	if (hitbox == sf::FloatRect(0.0f,0.0f,0.0f,0.0f) && sprite != nullptr)
	{
		return sprite->getLocalBounds();
	}
	return hitbox;
}

sf::Rect<float> Entity::GetHitBoxGlobalBounds() const
{
	auto origin = sprite->getOrigin();
	auto position = sprite->getPosition();

	auto bounds = GetHitBox();

	bounds.left += (position.x - origin.x);
	bounds.top += (position.y + origin.y);

	return bounds;
}

void Entity::SetHitbox(sf::Rect<float> hitbox)
{
	this->hitbox = hitbox;
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

	if (collisionEnabled)
	{
		Direction moveDirection = DirectionFromVector(direction);


		//auto tiles = GetTilesAroundEntity();

		//float distanceToWall = GetDistanceToWall(tiles, moveDirection, *sprite, map.GetTileSize(), { 0.0f,8.0f });
		float distanceToWall = GetDistanceToWall(moveDirection);
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

float Entity::GetDistanceToWall(Direction direction) const
{
	auto sprite = GetSprite();
	if (sprite == nullptr)
	{
		return INFINITY;
	}
	return GetDistanceToWall(direction, { 0.0f,static_cast<float>(sprite->getLocalBounds().height) });
}

float Entity::GetDistanceToWall(Direction direction, sf::Vector2f offset) const
{
	auto tiles = GetTilesAroundEntity();
	return GetDistanceToWall(tiles, direction, offset);
}

float Entity::GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction) const
{
	auto sprite = GetSprite();
	if (sprite == nullptr)
	{
		return INFINITY;
	}
	return GetDistanceToWall(tiles, direction, { 0.0f,static_cast<float>(sprite->getGlobalBounds().height) });
}

float Entity::GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Vector2f offset) const
{
	if (sprite == nullptr)
	{
		return INFINITY;
	}
	auto tileSize = map.GetTileSize();
	auto bounds = hitbox;

	if (bounds == Rect<float>(0.0f,0.0f,0.0f,0.0f))
	{
		bounds = sprite->getLocalBounds();
	}

	auto origin = sprite->getOrigin();
	auto position = sprite->getPosition();

	bounds.left += (position.x - origin.x);
	bounds.top += (position.y - origin.y);
	//auto bounds = sprite->getGlobalBounds();

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
	return result;
}

/*bool Entity::IsTouchingWall() const
{
	if (sprite == nullptr)
	{
		return false;
	}

	auto bounds = sprite->getTextureRect();

	return IsTouchingWall({0,static_cast<float>(bounds.height)});
}*/

bool Entity::IsTouchingWall(Vector2f offset) const
{
	if (sprite == nullptr)
	{
		return false;
	}
	auto bounds = hitbox;
	if (bounds == sf::Rect<float>(0,0,0,0))
	{
		bounds = sprite->getLocalBounds();
	}
	//auto bounds = sprite->getLocalBounds();
	auto origin = sprite->getOrigin();
	auto position = sprite->getPosition();

	bounds.left += (position.x - origin.x);
	bounds.top += (position.y - origin.y);

	//auto bounds2 = sprite->getGlobalBounds();

	bounds.left -= offset.x;
	bounds.top -= offset.y;

	auto tiles = GetTilesAroundEntity();

	auto tileSize = map.GetTileSize();

	auto bottomLeft = tiles.GetOffset();
	for (int x = 0; x < tiles.GetWidth(); x++)
	{
		for (int y = 0; y < tiles.GetHeight(); y++)
		{
			auto tile = tiles.GetRelative(x, y);
			if (tile == nullptr || tile->IsCollidable())
			{
				Rect<float> tileLocalBounds = Common::Sprites::centerPiece1.getGlobalBounds();
				Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x), tileLocalBounds.top + bottomLeft.y + (y * tileSize.y), tileLocalBounds.width, tileLocalBounds.height);
				if (Math::RectsIntersect(tileBounds,bounds))
				{
					return true;
				}
			}
		}
	}
	return false;
}

/*bool Entity::IsTouchingWall(sf::Rect<float> hitbox) const
{
	if (GetSprite() == nullptr)
	{
		return false;
	}

	float yOffset = static_cast<float>(-sprite->getLocalBounds().height);

	if (hitbox != Rect<float>(0,0,0,0))
	{
		yOffset = -hitbox.height;
	}

	return IsTouchingWall({ 0.0f,yOffset / 2}, hitbox);
}*/

void Entity::MoveCameraTo(sf::Vector2f position, float lerpAmount)
{
	sf::View cameraView = Common::MainWindow.getView();

	cameraView.setCenter(Math::VectorLerp(cameraView.getCenter(), position, lerpAmount));

	Common::MainWindow.setView(cameraView);
}

const WorldMap& Entity::GetMap() const
{
	return map;
}

Entity::~Entity()
{
	EnableRendering(false);
}




/*namespace
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
}*/