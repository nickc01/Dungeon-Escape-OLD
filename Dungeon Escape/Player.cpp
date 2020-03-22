#include "Player.h" //Contains the Player class that represents the player
#include "resource.h" //Contains the IDs of the game's resources
#include "Common.h" //Contains many common game functions and variables
#include "Math.h" //Contains many commonly used math functions
#include "MagicOrb.h" //Contains the MagicOrb class for shooting magic orbs
#include "Door.h" //Contains the Door class, which is where the player enters to win the game


using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

namespace
{
	float constexpr INVINCIBILITY_TIME = 2.0f; //How long the player is invincible when taking a hit
	float constexpr FLICKER_RATE = 1.0f / 24.0f; //How fast the player should flicker when taking a hit
	float constexpr ORB_SPAWN_RATE = 0.5f; //How fast the player can shoot magic orbs
}


ResourceTexture Player::playerSpriteSheet = ResourceTexture{PLAYER_SPRITE_SHEET}; //The texture resource for the player
Player* Player::currentPlayer = nullptr; //The singleton object for the player class

//Updates the player sprite
void Player::UpdateSprite()
{
	switch (travelDirection)
	{
		//If the player is traveling up
	case Direction::Up:
		//Set the sprite animation to be the Up Animation
		SetAnimationGroup("UP");
		break;
		//If the player is traveling right
	case Direction::Right:
		//Set the sprite animation to be the Right Animation
		SetAnimationGroup("RIGHT");
		break;
		//If the player is traveling down
	case Direction::Down:
		//Set the sprite animation to be the Down Animation
		SetAnimationGroup("DOWN");
		break;
		//If the player is traveling left
	case Direction::Left:
		//Set the sprite animation to be the Left Animation
		SetAnimationGroup("LEFT");
		break;
	}
}

//Gets the currently spawned player in the game
Player* Player::GetCurrentPlayer()
{
	return currentPlayer;
}

//Constructs a new player
Player::Player(const WorldMap& map, sf::Vector2f spawnPoint) :
	AnimatedEntity(map,true,1.0f / 12.0f)
{
	//Sets the current player in the game
	currentPlayer = this;

	//Add all the sprites to the animation lists
	EmplaceSprite("DOWN",playerSpriteSheet.GetTexture(), IntRect(0, 0, 8, 8));
	EmplaceSprite("DOWN", playerSpriteSheet.GetTexture(), IntRect(8, 0, 8, 8));
	EmplaceSprite("DOWN", playerSpriteSheet.GetTexture(), IntRect(16, 0, 8, 8));

	EmplaceSprite("RIGHT", playerSpriteSheet.GetTexture(), IntRect(0, 8, 8, 8));
	EmplaceSprite("RIGHT", playerSpriteSheet.GetTexture(), IntRect(8, 8, 8, 8));
	EmplaceSprite("RIGHT", playerSpriteSheet.GetTexture(), IntRect(16, 8, 8, 8));

	EmplaceSprite("UP", playerSpriteSheet.GetTexture(), IntRect(0, 16, 8, 8));
	EmplaceSprite("UP", playerSpriteSheet.GetTexture(), IntRect(8, 16, 8, 8));
	EmplaceSprite("UP", playerSpriteSheet.GetTexture(), IntRect(16, 16, 8, 8));

	EmplaceSprite("LEFT", playerSpriteSheet.GetTexture(), IntRect(0, 24, 8, 8));
	EmplaceSprite("LEFT", playerSpriteSheet.GetTexture(), IntRect(8, 24, 8, 8));
	EmplaceSprite("LEFT", playerSpriteSheet.GetTexture(), IntRect(16, 24, 8, 8));

	//Set the default animation to the Up animation
	SetAnimationGroup("UP");


	//Get the texture rect of the player sprite
	auto rect = GetSprite()->getTextureRect();

	//Set the render layer
	SetRenderLayer(10);

	//Set the player's position to the spawnpoint
	GetSprite()->setPosition(spawnPoint);

	//Set the player's origin to the center of the texture
	GetSprite()->setOrigin(rect.width / 2.0f, rect.height / 2.0f);

	//Set the camera to where the player is at
	MoveCameraTo(GetSprite()->getPosition());

	//Enable updating
	UpdateReceiver::SetActive(true);

	//Enable rendering
	Renderable::SetActive(true);
}


//Gets the player's traveling direction
Direction Player::GetPlayerDirection() const
{
	return travelDirection;
}

//Returns whether the player is moving or not
bool Player::IsMoving() const
{
	return moving;
}

//Gets the health of the player
int Player::GetHealth() const
{
	return health;
}

//Causes the player to take a hit and lose some health
void Player::TakeHit()
{
	//If the player is not invincible and it's health is greater than zero
	if (!invincible && health > 0)
	{
		//Decrease the player's health
		health--;
		//If the health is now zero
		if (health == 0)
		{
			//Disable rendering, hiding the player
			EnableRendering(false);
			//Break out
			return;
		}
		//Make the player temporarily invisible
		invincible = true;
		//Set the invincibility timer
		invincibilityTimer = INVINCIBILITY_TIME;
		//Set the flicker timer
		flickerTimer = FLICKER_RATE;
		//Set the sprite color to redish
		GetSprite()->setColor(Color(255,128,128));
	}
}

//Whether the player is alive or not
bool Player::IsAlive() const
{
	return health > 0 && reachedDoor == false;
}

//Whether the player has reached the door or not
bool Player::ReachedTheDoor() const
{
	return reachedDoor;
}

//The update loop of the player
void Player::Update(sf::Time dt)
{
	//If the player is not alive or it has reach the door already
	if (!IsAlive() || reachedDoor)
	{
		//Break out
		return;
	}
	//Get the time between this frame and the last
	auto time = dt.asSeconds();

	//Get the player's position
	auto playerPosition = GetSprite()->getPosition();

	//If the player is invincible
	if (invincible)
	{
		//Decrease the invincibility timer
		invincibilityTimer -= time;
		//Decrease the flicker timer
		flickerTimer -= time;
		//If the flicker timer is zero
		if (flickerTimer <= 0)
		{
			//Reset the flicker timer
			flickerTimer += FLICKER_RATE;
			//Get the color of the player
			auto spriteColor = GetSprite()->getColor();

			//Invert the transparency of the player
			spriteColor.a = 255 - spriteColor.a;

			//Set the sprite's new color
			GetSprite()->setColor(spriteColor);
		}

		//If the invincibility timer is zero0
		if (invincibilityTimer <= 0)
		{
			//Reset the timers
			invincibilityTimer = 0;
			flickerTimer = 0;
			//The player is no longer invincible
			invincible = false;

			//Reset the player's color
			GetSprite()->setColor(Color(255,255,255));
		}
	}

	//Move the camera towards the player
	MoveCameraTo(playerPosition, 2.5f * time);

	//Get the previous travel direction of the player
	Direction previousDirection = travelDirection;

	//Stores whether the player is moving or not
	bool isMoving = false;

	//If the Up or W keys are pressed
	if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W))
	{
		//Move the player up
		Move(0.0f,-60.0f * time);
		//Update the travel direction
		travelDirection = Direction::Up;
		//The player is moving
		isMoving = true;
	}
	//If the Down or S keys are pressed
	if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
	{
		//Move the player down
		Move(0.0f, 60.0f * time);
		//Update the travel direction
		travelDirection = Direction::Down;
		//The player is moving
		isMoving = true;
	}
	//If the Right or D keys are pressed
	if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
	{
		//Move the player right
		Move(60.0f * time, 0.0f);
		//Update the travel direction
		travelDirection = Direction::Right;
		//The player is moving
		isMoving = true;
	}
	//If the Left or A keys are pressed
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
	{
		//Move the player left
		Move(-60.0f * time, 0.0f);
		//Update the travel direction
		travelDirection = Direction::Left;
		//The player is moving
		isMoving = true;
	}

	//If the orb spawn timer is zero
	if (orbSpawnTimer <= 0.0f)
	{
		//If the left mouse button is pressed
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			//Reset the orb spawn timer
			orbSpawnTimer = ORB_SPAWN_RATE;

			//Spawn an orb at the player's position, and moving towards where the mouse was clicked
			MagicOrb::Fire(GetMap(), GetSprite()->getPosition(), Common::GetMouseWorldCoordinates() - GetSprite()->getPosition());
		}
		//If the space bar is pressed
		else if (Keyboard::isKeyPressed(Keyboard::Key::Space))
		{
			//Reset the orb spawn timer
			orbSpawnTimer = ORB_SPAWN_RATE;
			//Spawn an orb at the player's position, and moving towards where the player is looking
			MagicOrb::Fire(GetMap(), GetSprite()->getPosition(), travelDirection);
		}
	}
	//If the orb spawn timer is not zero
	else
	{
		//Decrease the orb spawn timer
		orbSpawnTimer -= time;
	}

	//Update whether the player is moving or not
	moving = isMoving;

	//If the player is moving
	if (moving)
	{
		//Update the current sprite
		UpdateSprite();
		//Move the animation forward
		UpdateAnimations(dt);
	}

	//If the player and the door intersect
	if (Math::SpritesIntersect(*GetSprite(),Door::GetDoor()->GetSprite()))
	{
		//The player has reached the door
		reachedDoor = true;
		//Disable rendering
		EnableRendering(false);
	}
}

Player::~Player()
{
	//If the singleton is set to this instance
	if (currentPlayer == this)
	{
		//Set it to null
		currentPlayer = nullptr;
	}
}
