#include "Entity.h" //Contains the Entity class
#include "Common.h" //Contains many common game functions and variables
#include "Math.h" //Contains many commonly used math functions

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

//Draws the entity to the screen
void Entity::Render(sf::RenderWindow& window)
{
	//If a sprite is specified
	if (sprite != nullptr)
	{
		//Draw the sprite
		window.draw(*sprite);
	}
}

//Gets all the world tiles that are nearby the entity
Array2D<BackgroundTile*> Entity::GetTilesAroundEntity() const
{
	//If no sprite is set
	if (sprite == nullptr)
	{
		//Return an empty array
		return Array2D<BackgroundTile*>(0, 0, nullptr);
	}

	//Get the position of the sprite
	auto position = sprite->getPosition();

	//Get the global bounds of the sprite
	auto entityBounds = sprite->getGlobalBounds();

	//Get the width of the bounds
	auto width = entityBounds.width;
	//Get the height of the bounds
	auto height = entityBounds.height;

	//Get the rightmost side of the bounds
	auto right = entityBounds.left + width;
	//Get the bottom side of the bounds
	auto bottom = entityBounds.top - height;
	//Get the leftmost side of the bounds
	auto left = entityBounds.left;
	//Get the top side of the bounds
	auto top = entityBounds.top;

	//Increase each side by a factor of 5
	top += (height * 5);
	bottom -= (height * 5);
	right += (width * 5);
	left -= (width * 5);

	//Get all the tiles within the specified bounds
	return map.GetTilesWithinRect(Rect<float>(left, top, right - left, top - bottom));
}

//Constructs a new entity
Entity::Entity(const WorldMap& map, bool collisionEnabled) :
	Entity(map,nullptr,collisionEnabled)
{

}

//Constructs a new entity with the specified sprite
Entity::Entity(const WorldMap& map, sf::Sprite* sprite,bool collisionEnabled) :
	map(map),
	collisionEnabled(collisionEnabled),
	sprite(sprite)
{

}

//Gets the currently set sprite
sf::Sprite* Entity::GetSprite()
{
	return sprite;
}

//Gets the currently set sprite
const sf::Sprite* Entity::GetSprite() const
{
	return sprite;
}

//Sets the entity's sprite
void Entity::SetSprite(sf::Sprite* newSprite)
{
	//If the sprite is the same as the old sprite
	if (sprite == newSprite)
	{
		//Return
		return;
	}
	//If the old sprite is null
	if (sprite == nullptr)
	{
		//Then simply update the current sprite
		sprite = newSprite;
	}
	//If the old sprite is not null
	else
	{
		//Get the necessary information on the old sprite
		auto previousPosition = sprite->getPosition();
		auto previousScale = sprite->getScale();
		auto previousOrigin = sprite->getOrigin();
		auto previousRotation = sprite->getRotation();
		auto previousColor = sprite->getColor();

		//Update the sprite
		sprite = newSprite;

		//If the new sprite is not null
		if (sprite != nullptr)
		{
			//Set the sprite's information to the old sprite information
			sprite->setPosition(previousPosition);
			sprite->setScale(previousScale);
			sprite->setOrigin(previousOrigin);
			sprite->setRotation(previousRotation);
			sprite->setColor(previousColor);
		}
	}
}

//Gets the collision mode of the entity
bool Entity::GetCollisionMode() const
{
	return collisionEnabled;
}

//Sets the collision mode of the entity
void Entity::SetCollisionMode(bool enabled)
{
	collisionEnabled = enabled;
}

//Gets the hitbox of the entity
sf::Rect<float> Entity::GetHitBox() const
{
	//If no hitbox has been specified and a sprite is already set
	if (hitbox == sf::FloatRect(0.0f,0.0f,0.0f,0.0f) && sprite != nullptr)
	{
		//Use the texture bounds instead
		return sprite->getLocalBounds();
	}
	return hitbox;
}

//Gets the hitbox of the entity in global coordinates
sf::Rect<float> Entity::GetHitBoxGlobalBounds() const
{
	//Get the origin of the sprite
	auto origin = sprite->getOrigin();
	//Get the position of the sprite
	auto position = sprite->getPosition();

	//Get the hitbox bounds
	auto bounds = GetHitBox();

	auto globalBounds = sprite->getGlobalBounds();

	//Add the position and origin to the bounds
	bounds.left += (position.x - origin.x);
	bounds.top += (position.y + origin.y);

	return bounds;
}

//Sets the entity's hitbox
void Entity::SetHitbox(sf::Rect<float> hitbox)
{
	this->hitbox = hitbox;
}

//Moves the sprite in the specified direction
void Entity::Move(float x, float y)
{
	Move({x,y});
}

//Moves the sprite in the specified direction
//NOTE : IN SFML, down mean we move up towards the bottom of the screen, and down means we move towards the top of the screen
void Entity::Move(sf::Vector2f direction)
{
	//If no sprite has been configured
	if (sprite == nullptr)
	{
		//Return out
		return;
	}

	//if the direction is not facing a cardinal direction
	if (direction.x != 0 && direction.y != 0)
	{
		//Get the x and y components of the vector
		auto directionComponents = Math::VectorComponents(direction);

		//Do two movements in each component direction
		Move(directionComponents[0]);
		Move(directionComponents[1]);
		return;
	}

	//If collision is enabled
	if (collisionEnabled)
	{
		//Convert the vector into a direction
		Direction moveDirection = DirectionFromVector(direction);

		//Get the distance to the nearest wall in the specified direction
		float distanceToWall = GetDistanceToWall(moveDirection);


		switch (moveDirection)
		{
			//If we are moving up
		case Direction::Up:
			//Clamp the y component to be less than or equal to the distance to the wall
			if (direction.y > distanceToWall)
			{
				direction.y = distanceToWall;
			}
			break;
			//If we are moving up
		case Direction::Right:
			//Clamp the xs component to be less than or equal to the distance to the wall
			if (direction.x > distanceToWall)
			{
				direction.x = distanceToWall;
			}
			break;
			//If we are moving up
		case Direction::Down:
			//Clamp the y component to be less than or equal to the distance to the wall
			if (direction.y < -distanceToWall)
			{
				direction.y = -distanceToWall;
			}
			break;
			//If we are moving up
		case Direction::Left:
			//Clamp the x component to be less than or equal to the distance to the wall
			if (direction.x < -distanceToWall)
			{
				direction.x = -distanceToWall;
			}
			break;
		}
	}

	//Move the sprite in the specified direction
	sprite->move(direction);
}

//Moves the sprite in the specified direction
void Entity::Move(Direction direction, int scalar)
{
	Move(VectorInDirection<float>(direction, scalar));
}

//Gets the distance to the nearest wall in the specified direction
float Entity::GetDistanceToWall(Direction direction) const
{
	auto sprite = GetSprite();
	if (sprite == nullptr)
	{
		return INFINITY;
	}
	return GetDistanceToWall(direction, { 0.0f, static_cast<float>(sprite->getTextureRect().height * 2) } );
}

//Gets the distance to the nearest wall in the specified direction, with the applied offset to the sprite
float Entity::GetDistanceToWall(Direction direction, sf::Vector2f offset) const
{
	auto tiles = GetTilesAroundEntity();
	return GetDistanceToWall(tiles, direction, offset);
}

//Gets the distance to the nearest wall in the specified direction, with the specified tiles to take into account
float Entity::GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction) const
{
	auto sprite = GetSprite();
	if (sprite == nullptr)
	{
		return INFINITY;
	}
	return GetDistanceToWall(tiles, direction, { 0.0f, static_cast<float>(sprite->getTextureRect().height * 2) });
}

//Gets the distance to the nearest wall in the specified direction, with the specified tiles to take into account and with the applied offset to the sprite
float Entity::GetDistanceToWall(Array2D<BackgroundTile*>& tiles, Direction direction, Vector2f offset) const
{
	//If no sprite has been set on the entity
	if (sprite == nullptr)
	{
		//Return infinity
		return INFINITY;
	}
	//Get the size of each tile in the map
	auto tileSize = map.GetTileSize();

	//Get the hitbox of the entity
	auto bounds = GetHitBoxGlobalBounds();

	//Subtract an offset if specified
	bounds.left -= offset.x;
	bounds.top -= offset.y;

	//The distance to the closest wall in the specified direction
	float nearestDistance = INFINITY;

	//If we are searching towards the left or down
	if (direction == Direction::Left || direction == Direction::Down)
	{
		//Start the nearest distance negative instead
		nearestDistance = -INFINITY;
	}

	//Get the bottom left corner position of the input tiles, which is specified in the 2D Tile Array's offset
	auto bottomLeft = tiles.GetOffset();

	//float result = 0.0f;

	//Loop over all the specified tiles
	for (int x = 0; x < tiles.GetWidth(); x++)
	{
		for (int y = 0; y < tiles.GetHeight(); y++)
		{
			//Get the tile at the index
			auto tile = tiles.GetRelative(x, y);
			//If the there is no tile at the position, or the tile is collidable, that means it is a wall
			if (tile == nullptr || tile->IsCollidable())
			{
				//Get the tile bounds of the center piece, since all other tiles share similar info
				Rect<float> wallBounds = Common::Sprites::centerPiece1.getGlobalBounds();

				//Compute the global bounds of the wall
				Rect<float> wallWorldBounds = Rect<float>(wallBounds.left + bottomLeft.x + (x * tileSize.x), wallBounds.top + bottomLeft.y + (y * tileSize.y), wallBounds.width, wallBounds.height);
				switch (direction)
				{
					//If we are looking in the upwards direction
				case Direction::Up:

					//First, check to see if the wall is even in range of the entity.
					if (wallWorldBounds.left < bounds.left + bounds.width && wallWorldBounds.left + wallWorldBounds.width > bounds.left)
					{
						//Then, we do two checks here,
						// First : we check to see if the wall tile is closer than the previously calculated one. This is how we determine the closest distance to the wall
						// Second : we check to make sure that the wall tile is even above the entity in the first place.
						if (wallWorldBounds.top - wallWorldBounds.height < nearestDistance && wallWorldBounds.top > bounds.top)
						{
							//If both checks are true, update the closest distance with the new wall we found
							nearestDistance = wallWorldBounds.top - wallWorldBounds.height;
						}
					}
					break;
					//If we are looking in the rightwards direction
				case Direction::Right:
					//First, check to see if the wall is even in range of the entity.
					if (wallWorldBounds.top > bounds.top - bounds.height && wallWorldBounds.top - wallWorldBounds.height < bounds.top)
					{
						//Then, we do two checks here,
						// First : we check to see if the wall tile is closer than the previously calculated one. This is how we determine the closest distance to the wall
						// Second : we check to make sure that the wall tile is even to the right of the entity in the first place.
						if (wallWorldBounds.left < nearestDistance && wallWorldBounds.left + wallWorldBounds.width > bounds.left + wallWorldBounds.width)
						{
							//If both checks are true, update the closest distance with the new wall we found
							nearestDistance = wallWorldBounds.left;
						}
					}
					break;
					//If we are looking in the downwards direction
				case Direction::Down:
					//First, check to see if the wall is even in range of the entity.
					if (wallWorldBounds.left < bounds.left + bounds.width && wallWorldBounds.left + wallWorldBounds.width > bounds.left)
					{
						//Then, we do two checks here,
						// First : we check to see if the wall tile is closer than the previously calculated one. This is how we determine the closest distance to the wall
						// Second : we check to make sure that the wall tile is even below the entity in the first place.
						if (wallWorldBounds.top > nearestDistance&& wallWorldBounds.top - wallWorldBounds.height < bounds.top - bounds.height)
						{
							//If both checks are true, update the closest distance with the new wall we found
							nearestDistance = wallWorldBounds.top;
						}
					}
					break;
					//If we are looking in the leftwards direction
				case Direction::Left:
					//First, check to see if the wall is even in range of the entity.
					if (wallWorldBounds.top > bounds.top - bounds.height && wallWorldBounds.top - wallWorldBounds.height < bounds.top)
					{
						//Then, we do two checks here,
						// First : we check to see if the wall tile is closer than the previously calculated one. This is how we determine the closest distance to the wall
						// Second : we check to make sure that the wall tile is even to the left of the entity in the first place.
						if (wallWorldBounds.left + wallWorldBounds.width > nearestDistance&& wallWorldBounds.left < bounds.left)
						{
							//If both checks are true, update the closest distance with the new wall we found
							nearestDistance = wallWorldBounds.left + wallWorldBounds.width;
						}
					}
					break;
				}
			}
		}
	}

	switch (direction)
	{
		//If we are looking in the upwards direction
	case Direction::Up:
		//Return the distance between the top of the entity and the bottom of the wall
		return nearestDistance - bounds.top;
	case Direction::Right:
		//Return the distance between the left side of the wall and the right side of the entity
		return nearestDistance - (bounds.left + bounds.width);
	case Direction::Down:
		//Return the distance between the bottom of the entity and the top of the wall
		return (bounds.top - bounds.height) - nearestDistance;
	case Direction::Left:
		//Return the distance between the left side of the entity and the right side of the wall
		return bounds.left - nearestDistance;
	default:
		//Default case if none are run
		return INFINITY;
	}
}

bool Entity::IsTouchingWall(Vector2f offset) const
{
	//If there is not sprite assigned
	if (sprite == nullptr)
	{
		//Return false
		return false;
	}

	//Get the hitbox of the entity
	auto bounds = GetHitBoxGlobalBounds();

	//Apply an offset if specified
	bounds.left -= offset.x;
	bounds.top -= offset.y;

	//Gets all the tiles surrounding the entity
	auto tiles = GetTilesAroundEntity();

	//Gets the size of each tile in the map
	auto tileSize = map.GetTileSize();

	//Gets the bottom left corner position of the tiles surrounding the entity
	auto bottomLeft = tiles.GetOffset();

	//Loop over all the tiles in the array
	for (int x = 0; x < tiles.GetWidth(); x++)
	{
		for (int y = 0; y < tiles.GetHeight(); y++)
		{
			//Get the tile at the index
			auto tile = tiles.GetRelative(x, y);
			//If the there is no tile at the position, or the tile is collidable, that means it is a wall
			if (tile == nullptr || tile->IsCollidable())
			{
				//Get the tile bounds of the center piece, since all other tiles share similar info
				Rect<float> wallBounds = Common::Sprites::centerPiece1.getGlobalBounds();

				//Compute the global bounds of the wall
				Rect<float> wallGlobalBounds = Rect<float>(wallBounds.left + bottomLeft.x + (x * tileSize.x), wallBounds.top + bottomLeft.y + (y * tileSize.y), wallBounds.width, wallBounds.height);
				//If the wall intersects with the entity
				if (Math::RectsIntersect(wallGlobalBounds,bounds))
				{
					//Return true
					return true;
				}
			}
		}
	}
	//If no collisions have occured, return false
	return false;
}

//Moves the camera to the specified position.
//If the lerp amount is less than one, then that determines the percentage on how far the camera should move towards the new position.
//For example if the lerp is 0.5f or 50%, then the camera will move 50% the distance between the old position and the new position
void Entity::MoveCameraTo(sf::Vector2f position, float lerpAmount)
{
	//Get the current camera view
	sf::View cameraView = Common::MainWindow.getView();

	//Linearly interpolate to the new position by the lerp amount
	cameraView.setCenter(Math::VectorLerp(cameraView.getCenter(), position, lerpAmount));

	//Set the new view
	Common::MainWindow.setView(cameraView);
}

//Gets the world map the entity is living in
const WorldMap& Entity::GetMap() const
{
	return map;
}

//The destructor for the entity
Entity::~Entity()
{
	//Disable rendering
	EnableRendering(false);
}