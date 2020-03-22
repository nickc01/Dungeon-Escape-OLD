#include "MagicOrb.h"
#include <algorithm> //Contains many commonly used algorithms such as std::sort and std::find
#include "resource.h" //Contains the IDs of the game's resources
#include "Math.h" //Contains many commonly used math functions
#include "Common.h" //Contains many common game functions and variables

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

namespace
{
	constexpr float TRAVEL_SPEED = 75.0f; //How fast the magic orb should travel
	constexpr float SPAWN_INVINCIBILITY_TIME = 0.2f; //The amount of invinciblity time before the magic orb responds to wall collisions
	constexpr float LIFETIME = 3.0f; //How long the orb will live before it's automatically destroyed
}

ResourceTexture MagicOrb::orbTexture{POWER_ORB}; //The texture resource for the magic orb
recursive_mutex MagicOrb::orbsMutex{}; //The mutex of the list of spawned orbs
std::list<std::shared_ptr<MagicOrb>> MagicOrb::SpawnedOrbs{}; //The list of spawned orbs

//Constructs a new magic orb
MagicOrb::MagicOrb(const WorldMap& map, Vector2f spawnPoint, Vector2f direction) :
	Entity(map,false),
	map(map),
	orbSprite(orbTexture.GetTexture()),
	direction(direction)
{
	//Reset the invincibility timer
	spawnInvincibilityTimer = SPAWN_INVINCIBILITY_TIME;

	//Get the texture size of the magic orb texture
	auto textureSize = orbTexture.GetTexture().getSize();

	//Set the sprite's origin to be the center of the texture
	orbSprite.setOrigin(Vector2f(textureSize.x / 2u,textureSize.y / 2u));
	//Set the position of the sprite to the spawnpoint
	orbSprite.setPosition(spawnPoint);
	//Set the sprite to be the orb sprite
	SetSprite(&orbSprite);

	//Set the render layer
	SetRenderLayer(200);

	//Make the orb semi-transparent
	orbSprite.setColor({ 255,255,255,128 });
}

//Gets the mutex for the list of spawned orbs
std::recursive_mutex& MagicOrb::GetOrbListMutex()
{
	return orbsMutex;
}

//Gets the list of all the spawned orbs
const std::list<std::shared_ptr<MagicOrb>>& MagicOrb::GetAllOrbs()
{
	return SpawnedOrbs;
}

//Fires a new magic orb
std::shared_ptr<MagicOrb> MagicOrb::Fire(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction)
{
	//Create the new magic orb object
	auto orbObject = shared_ptr<MagicOrb>(new MagicOrb(map, spawnPoint,Math::NormalizeVector(direction)));

	//Lock the orb list mutex
	unique_lock<recursive_mutex> lock(orbsMutex);
	//Add the new orb object to the list
	SpawnedOrbs.push_back(orbObject);

	//Enable rendering for the orb
	orbObject->EnableRendering();
	//Enable updating for the orb
	orbObject->EnableUpdating();

	//Return the new orb
	return orbObject;
}

//Fires a new magic orb
std::shared_ptr<MagicOrb> MagicOrb::Fire(const WorldMap& map, sf::Vector2f spawnPoint, Direction direction)
{
	//Convert the direction enum to a vector
	auto vectorDirection = VectorInDirection<float>(direction, 1.0f);

	//Flip the vertical direction because SFML has the up-down directions flipped
	vectorDirection.y = -vectorDirection.y;

	//Create the new orb
	return Fire(map, spawnPoint, vectorDirection);
}

//Destroys the orb
void MagicOrb::Destroy()
{
	//If the orb is still alive
	if (alive)
	{
		alive = false;
		//Disable rendering
		EnableRendering(false);
		//Disable updating
		EnableUpdating(false);
		//Lock the orb list mutex
		unique_lock<recursive_mutex> lock{ orbsMutex };

		//Remove the spawned orb from the list
		SpawnedOrbs.remove_if([self = this](std::shared_ptr<MagicOrb>& orb)
			{
				return orb.get() == self;
			});
	}
}

//Renders the magic orb to the screen
void MagicOrb::Render(sf::RenderWindow& window)
{
	//If the orb is alive
	if (alive)
	{
		//Draw it to the screen
		window.draw(orbSprite);
	}
}

//The update function of the magic orb
void MagicOrb::Update(sf::Time dt)
{
	//If the orb is alive
	if (alive)
	{
		//Get the time between the last frame and the current frame
		float time = dt.asSeconds();

		//Rotate the sprite
		orbSprite.rotate(180.0f * time);

		//Move the sprite in the travel direction
		orbSprite.move({direction.x * TRAVEL_SPEED * time,direction.y * TRAVEL_SPEED * time});

		//Increase the lifetime counter
		lifeTimeCounter += time;

		//If the orb has reached the end of its lifetime
		if (lifeTimeCounter >= LIFETIME)
		{
			//Destroy it
			Destroy();
			//Break out
			return;
		}

		//If the orb is no longer invincible
		if (spawnInvincibilityTimer <= 0.0f)
		{
			//If the orb is colliding with a wall
			if (IsTouchingWall({ 0,GetHitBox().height / 2 }))
			{
				//Destroy it
				Destroy();
			}
		}
		//If the orb is still invincible
		else
		{
			//Decrease the orb's invincibility timer
			spawnInvincibilityTimer -= time;
		}
	}
}
