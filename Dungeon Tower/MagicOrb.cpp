#include "MagicOrb.h"
#include <algorithm>
#include "resource.h"
#include "Math.h"
#include "Common.h"

using namespace std;
using namespace sf;

namespace
{
	constexpr float TRAVEL_SPEED = 75.0f;
	constexpr float SPAWN_INVINCIBILITY_TIME = 0.2f;
	constexpr float LIFETIME = 3.0f;
}

ResourceTexture MagicOrb::orbTexture{POWER_ORB};
recursive_mutex MagicOrb::orbsMutex{};
std::list<std::shared_ptr<MagicOrb>> MagicOrb::SpawnedOrbs{};


MagicOrb::MagicOrb(const WorldMap& map, Vector2f spawnPoint, Vector2f direction) :
	Entity(map,false),
	map(map),
	orbSprite(orbTexture.GetTexture()),
	direction(direction)
{
	spawnInvincibilityTimer = SPAWN_INVINCIBILITY_TIME;
	auto textureSize = orbTexture.GetTexture().getSize();
	orbSprite.setOrigin(Vector2f(textureSize.x / 2u,textureSize.y / 2u));
	orbSprite.setPosition(spawnPoint);
	SetSprite(&orbSprite);

	SetRenderLayer(200);

	orbSprite.setColor({ 255,255,255,128 });

	auto spriteBounds = GetSprite()->getLocalBounds();
	//SetHitbox({ spriteBounds.left - 3, spriteBounds.top + 3,spriteBounds.width + 3,spriteBounds.height + 3 });
}

std::recursive_mutex& MagicOrb::GetOrbListMutex()
{
	return orbsMutex;
}

const std::list<std::shared_ptr<MagicOrb>>& MagicOrb::GetAllOrbs()
{
	return SpawnedOrbs;
}

std::shared_ptr<MagicOrb> MagicOrb::Fire(const WorldMap& map, sf::Vector2f spawnPoint, sf::Vector2f direction)
{
	auto orbObject = shared_ptr<MagicOrb>(new MagicOrb(map, spawnPoint,Math::NormalizeVector(direction)));
	unique_lock<recursive_mutex> lock(orbsMutex);
	SpawnedOrbs.push_back(orbObject);

	orbObject->EnableRendering();
	orbObject->EnableUpdating();

	return orbObject;
}

std::shared_ptr<MagicOrb> MagicOrb::Fire(const WorldMap& map, sf::Vector2f spawnPoint, Direction direction)
{
	auto vectorDirection = VectorInDirection<float>(direction, 1.0f);

	vectorDirection.y = -vectorDirection.y;

	return Fire(map, spawnPoint, vectorDirection);
}

void MagicOrb::Destroy()
{
	if (alive)
	{
		alive = false;
		EnableRendering(false);
		EnableUpdating(false);
		unique_lock<recursive_mutex> lock{ orbsMutex };

		SpawnedOrbs.remove_if([self = this](std::shared_ptr<MagicOrb>& orb)
			{
				return orb.get() == self;
			});
	}
}

void MagicOrb::Render(sf::RenderWindow& window)
{
	if (alive)
	{
		window.draw(orbSprite);
	}
}

void MagicOrb::Update(sf::Time dt)
{

	if (alive)
	{
		float time = dt.asSeconds();

		orbSprite.rotate(180.0f * time);

		orbSprite.move({direction.x * TRAVEL_SPEED * time,direction.y * TRAVEL_SPEED * time});

		lifeTimeCounter += time;

		if (lifeTimeCounter >= LIFETIME)
		{
			Destroy();
			return;
		}

		if (spawnInvincibilityTimer <= 0.0f)
		{
			if (IsTouchingWall({ 0,-GetHitBox().height / 2 }))
			{
				Destroy();
			}
		}
		else
		{
			spawnInvincibilityTimer -= time;
		}
	}
}
