#include "Player.h"
#include "resource.h"
#include "Common.h"
#include "Math.h"


using namespace std;
using namespace sf;

namespace
{
	constexpr float AnimationSpeed = 1.0f / 12.0f;

	void MoveCameraTo(sf::Vector2f position, float lerpAmount = 1.0f);

	float GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Sprite sprite, Vector2u tileSize,Vector2f offset = {0.0f,0.0f});
}


ResourceTexture Player::playerSpriteSheet = ResourceTexture{PLAYER_SPRITE_SHEET};

void Player::UpdateSprite()
{
	switch (travelDirection)
	{
	case Direction::Up:
		SetSprite(upSprites[spriteIndex]);
		break;
	case Direction::Right:
		SetSprite(rightSprites[spriteIndex]);
		break;
	case Direction::Down:
		SetSprite(downSprites[spriteIndex]);
		break;
	case Direction::Left:
		SetSprite(leftSprites[spriteIndex]);
		break;
	default:
		break;
	}
}

Array2D<BackgroundTile*> Player::GetTilesAroundPlayer() const
{
	auto playerRect = currentSprite->getGlobalBounds();

	auto width = playerRect.width;
	auto height = playerRect.height;

	auto right = playerRect.left + width;
	auto bottom = playerRect.top - height;
	auto left = playerRect.left;
	auto top = playerRect.top;

	//playerRect.left -= (playerRect.width * 3);
	//playerRect.top += (playerRect.width * 3);

	//playerRect.width *= 6;
	//playerRect.height *= 6;

	top += (height * 5);
	bottom -= (height * 5);
	right += (width * 5);
	left -= (width * 5);

	return map.GetTilesWithinRect(Rect<float>(left,top,right - left,top - bottom));
}

Player::Player(const WorldMap& map, sf::Vector2f spawnPoint) :
	UpdateReceiver(false),
	map(map)
{
	downSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(0, 0, 8, 8));
	downSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(8, 0, 8, 8));
	downSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(16, 0, 8, 8));

	rightSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(0, 8, 8, 8));
	rightSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(8, 8, 8, 8));
	rightSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(16, 8, 8, 8));

	upSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(0, 16, 8, 8));
	upSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(8, 16, 8, 8));
	upSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(16, 16, 8, 8));

	leftSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(0, 24, 8, 8));
	leftSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(8, 24, 8, 8));
	leftSprites.emplace_back(playerSpriteSheet.GetTexture(), IntRect(16, 24, 8, 8));

	currentSprite = &upSprites[0];

	//auto textureSize = currentSprite->getTexture()->getSize();
	auto rect = currentSprite->getTextureRect();

	SetRenderLayer(10);

	currentSprite->setPosition(spawnPoint);
	currentSprite->setOrigin(rect.width / 2.0f, rect.height / 2.0f);

	//playerSprite.setPosition(spawnPoint);
	MoveCameraTo(currentSprite->getPosition());

	UpdateReceiver::SetActive(true);
}

void Player::Render(sf::RenderWindow& window) const
{
	if (currentSprite != nullptr)
	{
		window.draw(*currentSprite);
	}
	//window.draw(playerSprite);
}

void Player::Update(sf::Time dt)
{
	auto time = dt.asSeconds();

	auto playerPosition = currentSprite->getPosition();

	auto tiles = GetTilesAroundPlayer();

	MoveCameraTo(playerPosition, 2.5f * time);

	Direction previousDirection = travelDirection;

	moving = false;

	if (Keyboard::isKeyPressed(Keyboard::Key::Up))
	{
		Move(0.0f,-64.0f * time);
		travelDirection = Direction::Up;
		moving = true;
		//view.move(0, -200.0f * dt.asSeconds());
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Down))
	{
		Move(0.0f, 64.0f * time);
		travelDirection = Direction::Down;
		moving = true;
		//view.move(0, 200.0f * dt.asSeconds());
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Right))
	{
		Move(64.0f * time, 0.0f);
		travelDirection = Direction::Right;
		moving = true;
		//view.move(200.0f * dt.asSeconds(), 0);
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		Move(-64.0f * time, 0.0f);
		travelDirection = Direction::Left;
		moving = true;
		//view.move(-200.0f * dt.asSeconds(), 0);
	}
	if (moving)
	{
		animationTimer += time;

		if (previousDirection != travelDirection || animationTimer >= AnimationSpeed)
		{
			if (animationTimer >= AnimationSpeed)
			{
				animationTimer -= AnimationSpeed;
				if (indexForward)
				{
					spriteIndex++;
					if (spriteIndex == upSprites.size() - 1)
					{
						//spriteIndex = upSprites.size() - 1;
						indexForward = false;
					}
				}
				else
				{
					spriteIndex--;
					if (spriteIndex == 0)
					{
						indexForward = true;
					}
				}
				/*if (++spriteIndex >= upSprites.size())
				{
					spriteIndex = 0;
				}*/
			}
			UpdateSprite();
		}
	}
}

void Player::SetSprite(sf::Sprite& sprite)
{
	if (currentSprite == nullptr)
	{
		currentSprite = &sprite;
	}
	else
	{
		auto previousPosition = currentSprite->getPosition();
		auto previousScale = currentSprite->getScale();
		auto previousOrigin = currentSprite->getOrigin();

		currentSprite = &sprite;

		currentSprite->setPosition(previousPosition);
		currentSprite->setScale(previousScale);
		currentSprite->setOrigin(previousOrigin);
	}
}

sf::Sprite* Player::GetSprite()
{
	return currentSprite;
}

const sf::Sprite* Player::GetSprite() const
{
	return currentSprite;
}

Direction Player::GetPlayerDirection() const
{
	return travelDirection;
}

void Player::Move(float x, float y)
{
	Move({x,y});
}

void Player::Move(sf::Vector2f direction)
{
	Direction moveDirection = DirectionFromVector(direction);


	auto tiles = GetTilesAroundPlayer();

	float distanceToWall = GetDistanceToWall(tiles, moveDirection, *currentSprite, map.GetTileSize(), {0.0f,8.0f});

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

	currentSprite->move(direction);
}

void Player::GetPlayerSpriteEdge(Direction direction, bool worldCoords) const
{
	sf::FloatRect rect;
	if (worldCoords)
	{
		rect = currentSprite->getGlobalBounds();
	}
	else
	{
		rect = currentSprite->getLocalBounds();
	}
}






namespace
{
	void MoveCameraTo(sf::Vector2f position, float lerpAmount)
	{
		sf::View cameraView = Common::MainWindow.getView();

		cameraView.setCenter(Math::VectorLerp(cameraView.getCenter(),position,lerpAmount));

		Common::MainWindow.setView(cameraView);
	}


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
						Rect<float> tileBounds = Rect<float>(tileLocalBounds.left + bottomLeft.x + (x * tileSize.x),tileLocalBounds.top + bottomLeft.y + (y * tileSize.y),tileLocalBounds.width,tileLocalBounds.height);
						//tileBounds.left += (x * tileSize.x);
						//tileBounds.top += (x * tileSize.x);
						//Rect<float> tileBounds = Rect<float>((x * tileSize.x) + previousOffset.x, (y * tileSize.y) + previousOffset.y, tileSize.x, tileSize.y);
						//auto tileBounds = tiles[{x, y}]->GetSprite().getGlobalBounds();

						if (tileBounds.left < bounds.left + bounds.width && tileBounds.left + tileBounds.width > bounds.left)
						{
							if (tileBounds.top > nearestTileNumber && tileBounds.top - tileBounds.height < bounds.top - bounds.height)
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
							if (tileBounds.left + tileBounds.width > nearestTileNumber && tileBounds.left < bounds.left)
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
