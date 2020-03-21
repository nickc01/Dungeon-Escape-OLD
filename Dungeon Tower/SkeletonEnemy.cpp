#include "SkeletonEnemy.h"
#include "Common.h"
#include "Math.h"
#include "Player.h"
#include "MagicOrb.h"

using namespace std;
using namespace sf;

namespace
{
	constexpr float REFRESH_TIME = 1.0f;
	constexpr float STUN_DECELERATION_SPEED = 40.0f;
	constexpr float STUN_DURATION = 1.5f;
	constexpr float STUN_FORCE = 60.0f;
}

ResourceTexture SkeletonEnemy::skeletonSheet{SKELETON_SHEET};

SkeletonEnemy::SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint) :
	Enemy(map)
{
	EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8));
	EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8));
	EmplaceSprite("UP", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8));

	EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8));
	EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8));
	EmplaceSprite("RIGHT", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8));

	EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(0, 0, 6, 8));
	EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(7, 0, 6, 8));
	EmplaceSprite("DOWN", skeletonSheet.GetTexture(), IntRect(14, 0, 6, 8));

	EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(0, 9, 6, 8));
	EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(7, 9, 6, 8));
	EmplaceSprite("LEFT", skeletonSheet.GetTexture(), IntRect(14, 9, 6, 8));

	SetSpriteGroup("DOWN");

	auto rect = GetSprite()->getTextureRect();

	SetRenderLayer(8);

	GetSprite()->setPosition(spawnPoint);
	GetSprite()->setOrigin(rect.width / 2.0f, rect.height / 2.0f);


	UpdateReceiver::SetActive(true);
	Renderable::SetActive(true);
}

void SkeletonEnemy::Update(sf::Time dt)
{
	if (!IsAlive())
	{
		return;
	}
	float time = dt.asSeconds();
	if (stunned)
	{
		stunTimer -= time;

		if (stunTimer <= 0.0f)
		{
			GetSprite()->setColor(Color(255,255,255));
			stunned = false;
		}
		else
		{
			if (stunDirection.x != 0 && stunDirection.y != 0)
			{
				Move(stunDirection * time);

				auto length = Math::GetVectorLength(stunDirection);

				auto newLength = length - (STUN_DECELERATION_SPEED * time);

				if (newLength < 0.0f)
				{
					newLength = 0.0f;
				}

				stunDirection = Math::NormalizeVector(stunDirection, newLength);
			}
		}
	}
	else
	{
		if (AsyncResult != nullptr && AsyncResult->done)
		{
			pathToPlayer = AsyncResult->result.Path;
			//pathIndex = 0;
			AsyncResult = nullptr;

			if (pathToPlayer.size() > 0)
			{
				pathToPlayer.pop_front();
			}
		}

		updateTimer += time;
		if (updateTimer >= REFRESH_TIME || (pathToPlayer.size() == 0 && updateTimer > 0.25f))
		{
			updateTimer -= REFRESH_TIME;

			if (AsyncResult == nullptr)
			{
				AsyncResult = GeneratePathToPlayerAsync();
			}
		}

		if (pathToPlayer.size() > 0)
		{
			Vector2f distanceToNextPoint = pathToPlayer.front() - GetSprite()->getPosition();

			Vector2f amountToMove = Math::NormalizeVector(distanceToNextPoint, movementSpeed * time);

			Move(amountToMove);

			if (amountToMove == Vector2f(0.0f, 0.0f))
			{
				SetSpriteGroup("DOWN");
			}
			else
			{
				switch (Math::ApproxDirectionOfVector(distanceToNextPoint))
				{
				case Direction::Up:
					SetSpriteGroup("UP");
					GetSprite()->setScale({ 1.0f, 1.0f });
					break;
				case Direction::Right:
					SetSpriteGroup("RIGHT");
					GetSprite()->setScale({ 1.0f, 1.0f });
					break;
				case Direction::Down:
					SetSpriteGroup("DOWN");
					GetSprite()->setScale({ 1.0f, 1.0f });
					break;
				case Direction::Left:
					SetSpriteGroup("LEFT");
					GetSprite()->setScale({ -1.0f, 1.0f });
					break;
				}
			}

			UpdateAnimations(dt);

			if (Math::DistanceBetweenVectors(pathToPlayer.front(), GetSprite()->getPosition()) <= 1.0f)
			{
				pathToPlayer.pop_front();
			}
		}

		for (auto orb : MagicOrb::GetAllOrbs())
		{

			if (Math::EntitiesIntersect(*this, *orb))
			{
				health--;
				if (health == 0)
				{
					SetSprite(nullptr);
					return;
				}
				stunned = true;
				stunDirection = Math::NormalizeVector(GetSprite()->getPosition() - orb->GetSprite()->getPosition(), STUN_FORCE);
				stunTimer = STUN_DURATION;
				GetSprite()->setColor(Color(128, 128, 255));
			}
		}

		Player* player = Player::GetCurrentPlayer();

		if (player != nullptr && player->IsAlive() && Common::SpritesIntersect(*player->GetSprite(), *GetSprite()))
		{
			player->TakeHit();
		}
	}
}

bool SkeletonEnemy::IsStunned() const
{
	return stunned;
}

int SkeletonEnemy::GetHealth() const
{
	return health;
}

bool SkeletonEnemy::IsAlive() const
{
	return health > 0;
}
