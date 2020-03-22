#include "SkeletonEnemy.h" //Contains the SkeletonEnemy class
#include "Common.h" //Contains many common game functions and variables
#include "Math.h" //Contains many commonly used math functions
#include "Player.h" //Contains the Player class that represents the player
#include "MagicOrb.h"

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

namespace
{
	constexpr float REFRESH_TIME = 1.0f; //How fast the enemy skeleton should refresh its path to the player
	constexpr float STUN_DECELERATION_SPEED = 40.0f; //How fast the enemy skeletons stun movement should be decelerated
	constexpr float STUN_DURATION = 1.5f; //How long the enemy will remain in the stunned state
	constexpr float STUN_FORCE = 60.0f; //How fast the enemy should bounce back when stunned
}

ResourceTexture SkeletonEnemy::skeletonSheet{SKELETON_SHEET}; //The resource texture of the skeleton texture sheet

SkeletonEnemy::SkeletonEnemy(const WorldMap& map, sf::Vector2f spawnPoint) :
	Enemy(map)
{
	//Setup the animation sheet
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

	//Set the down animation as the default animation 
	SetAnimationGroup("DOWN");

	//Get the texture rect of the skeleton sprite
	auto rect = GetSprite()->getTextureRect();

	//Set the render layer
	SetRenderLayer(8);

	//Set the skeleton to the specified spawnpoint
	GetSprite()->setPosition(spawnPoint);

	//Set the sprite's origin to the center of the texture
	GetSprite()->setOrigin(rect.width / 2.0f, rect.height / 2.0f);

	//Enable updating
	UpdateReceiver::SetActive(true);

	//Enable rendering
	Renderable::SetActive(true);
}

//The update loop for the skeleton
void SkeletonEnemy::Update(sf::Time dt)
{
	//If the enemy is no longer alive
	if (!IsAlive())
	{
		//Break out
		return;
	}
	//Get the time between the last frame and this frame
	float time = dt.asSeconds();
	//If the enemy is stunned
	if (stunned)
	{
		//Decrease the stun timer
		stunTimer -= time;

		//If the stun timer is zero
		if (stunTimer <= 0.0f)
		{
			//Set the sprite's color back to normal
			GetSprite()->setColor(Color(255,255,255));
			//The enemy is no longer stunned
			stunned = false;
		}
		//If the enemy is still being stunned
		else
		{
			//If the stun direction is non-zero
			if (stunDirection.x != 0 && stunDirection.y != 0)
			{
				//Move the sprite in the stun direction
				Move(stunDirection * time);

				//Get the length of the stun direction
				auto length = Math::GetVectorLength(stunDirection);

				//Decrease the length of the stun direction to slow the enemy down
				auto newLength = length - (STUN_DECELERATION_SPEED * time);

				//If the new length is less than zero
				if (newLength < 0.0f)
				{
					//Clamp it to zero
					newLength = 0.0f;
				}

				//Set the stun direction to the new length
				stunDirection = Math::NormalizeVector(stunDirection, newLength);
			}
		}
	}
	//If the enemy is not stunned
	else
	{
		//If the enemy is awaiting pathfinding results and the results are in
		if (AsyncResult != nullptr && AsyncResult->done)
		{
			//Update the path to the player
			pathToPlayer = AsyncResult->result.Path;
			//Set the results to null since we got the new path now
			AsyncResult = nullptr;

			//If the path to the player is greater than zero
			if (pathToPlayer.size() > 0)
			{
				//Remove the first element, since that is the position of the enemy anyway
				pathToPlayer.pop_front();
			}
		}

		//Increase the update timer
		updateTimer += time;

		//If either :
		// A : The update timer is equal to or greater than the refresh time
		// B : There is no more path to travel and the update timer is greater than 0.25 seconds
		if (updateTimer >= REFRESH_TIME || (pathToPlayer.size() == 0 && updateTimer > 0.25f))
		{
			//Reset the update timer
			updateTimer = 0.0f;

			//If we are not awaiting any results yet
			if (AsyncResult == nullptr)
			{
				//Generate a path asynchronously and wait for the results
				AsyncResult = GeneratePathToPlayerAsync();
			}
		}

		//If there is more path to travel
		if (pathToPlayer.size() > 0)
		{
			//Move towards the next point in the path
			Vector2f distanceToNextPoint = pathToPlayer.front() - GetSprite()->getPosition();
			Vector2f amountToMove = Math::NormalizeVector(distanceToNextPoint, movementSpeed * time);

			//Move towards the points
			Move(amountToMove);

			//If the enemy is no longer moving anywhere
			if (amountToMove == Vector2f(0.0f, 0.0f))
			{
				//Set the default animation group to down
				SetAnimationGroup("DOWN");
			}
			//If the enemy is moving
			else
			{
				//Get approximately which direction the enemy is heading in
				switch (Math::ApproxDirectionOfVector(distanceToNextPoint))
				{
					//If the enemy is traveling up
				case Direction::Up:
					SetAnimationGroup("UP"); //Use the walking up sanimation
					GetSprite()->setScale({ 1.0f, 1.0f }); //Reset the scaling to normal
					break;

					//If the enemy is traveling right
				case Direction::Right:
					SetAnimationGroup("RIGHT"); //Use the walking right animation
					GetSprite()->setScale({ 1.0f, 1.0f }); //Reset the scaling to normal
					break;

					//If the enemy is traveling down
				case Direction::Down:
					SetAnimationGroup("DOWN"); //Use the walking down animation
					GetSprite()->setScale({ 1.0f, 1.0f }); //Reset the scaling to normal
					break;

					//If the enemy is traveling left
				case Direction::Left:
					SetAnimationGroup("LEFT");//Use the walking left animation
					GetSprite()->setScale({ -1.0f, 1.0f }); //Flip the sprite to the left
					break;
				}
			}

			//Move the animations forwards
			UpdateAnimations(dt);

			//If we are close enough to the next point in the path
			if (Math::DistanceBetweenVectors(pathToPlayer.front(), GetSprite()->getPosition()) <= 1.0f)
			{
				//Remove the point and move on to the next
				pathToPlayer.pop_front();
			}
		}

		//Loop over all the spawned orbs that have been fired by the player
		for (auto orb : MagicOrb::GetAllOrbs())
		{
			//If the enemy and the orb intersects with each other
			if (Math::EntitiesIntersect(*this, *orb))
			{
				//Decrease the enemy's health
				health--;
				//If the health is zero
				if (health == 0)
				{
					//Make the enemy invisible
					SetSprite(nullptr);
					//Break out
					return;
				}
				//Make the enemy stunned
				stunned = true;
				//Make the enemy move away from the orb
				stunDirection = Math::NormalizeVector(GetSprite()->getPosition() - orb->GetSprite()->getPosition(), STUN_FORCE);
				//Reset the stun timer
				stunTimer = STUN_DURATION;
				//Make the enemy sprite blue
				GetSprite()->setColor(Color(128, 128, 255));
			}
		}

		//Get the current player in the scene
		Player* player = Player::GetCurrentPlayer();

		//If all three are true:
		//A : The player exists
		//B : The player is alive
		//C : The enemy and the player intersect with each other
		if (player != nullptr && player->IsAlive() && Common::SpritesIntersect(*player->GetSprite(), *GetSprite()))
		{
			//The player should take a hit and lose some health
			player->TakeHit();
		}
	}
}

//Whether the enemy is stunned or not
bool SkeletonEnemy::IsStunned() const
{
	return stunned;
}

//Returns how much health the enemy has
int SkeletonEnemy::GetHealth() const
{
	return health;
}

//Whether the enemy is alive or not
bool SkeletonEnemy::IsAlive() const
{
	return health > 0;
}
