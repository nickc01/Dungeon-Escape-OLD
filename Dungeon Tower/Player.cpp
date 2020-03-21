#include "Player.h"
#include "resource.h"
#include "Common.h"
#include "Math.h"
#include "MagicOrb.h"
#include "Door.h"


using namespace std;
using namespace sf;

namespace
{
	float constexpr INVINCIBILITY_TIME = 2.0f;
	float constexpr FLICKER_RATE = 1.0f / 24.0f;
	float constexpr ORB_SPAWN_RATE = 0.5f;
}


ResourceTexture Player::playerSpriteSheet = ResourceTexture{PLAYER_SPRITE_SHEET};
Player* Player::currentPlayer = nullptr;

void Player::UpdateSprite()
{
	switch (travelDirection)
	{
	case Direction::Up:
		SetSpriteGroup("UP");
		//SetSprite(&upSprites[spriteIndex]);
		break;
	case Direction::Right:
		SetSpriteGroup("RIGHT");
		//SetSprite(&rightSprites[spriteIndex]);
		break;
	case Direction::Down:
		SetSpriteGroup("DOWN");
		//SetSprite(&downSprites[spriteIndex]);
		break;
	case Direction::Left:
		SetSpriteGroup("LEFT");
		//SetSprite(&leftSprites[spriteIndex]);
		break;
	default:
		break;
	}
}

Player* Player::GetCurrentPlayer()
{
	return currentPlayer;
}

Player::Player(const WorldMap& map, sf::Vector2f spawnPoint) :
	AnimatedEntity(map,true,1.0f / 12.0f)
{
	currentPlayer = this;

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

	SetSpriteGroup("UP");


	auto rect = GetSprite()->getTextureRect();

	SetRenderLayer(10);

	GetSprite()->setPosition(spawnPoint);
	GetSprite()->setOrigin(rect.width / 2.0f, rect.height / 2.0f);

	MoveCameraTo(GetSprite()->getPosition());

	UpdateReceiver::SetActive(true);
	Renderable::SetActive(true);
}

Direction Player::GetPlayerDirection() const
{
	return travelDirection;
}

bool Player::IsMoving() const
{
	return moving;
}

int Player::GetHealth() const
{
	return health;
}

void Player::TakeHit()
{
	if (!invincible && health > 0)
	{
		health--;
		if (health == 0)
		{
			EnableRendering(false);
			return;
		}
		invincible = true;
		invincibilityTimer = INVINCIBILITY_TIME;
		flickerTimer = FLICKER_RATE;
		GetSprite()->setColor(Color(255,128,128));
	}
}

bool Player::IsAlive() const
{
	return health > 0 && reachedDoor == false;
}

bool Player::ReachedTheDoor() const
{
	return reachedDoor;
}

void Player::Update(sf::Time dt)
{
	if (!IsAlive() || reachedDoor)
	{
		return;
	}
	auto time = dt.asSeconds();

	auto playerPosition = GetSprite()->getPosition();

	if (invincible)
	{
		invincibilityTimer -= time;
		flickerTimer -= time;
		if (flickerTimer <= 0)
		{
			flickerTimer += FLICKER_RATE;
			auto spriteColor = GetSprite()->getColor();
			spriteColor.a = 255 - spriteColor.a;
			GetSprite()->setColor(spriteColor);
		}

		if (invincibilityTimer <= 0)
		{
			invincibilityTimer = 0;
			flickerTimer = 0;
			invincible = false;
			GetSprite()->setColor(Color(255,255,255));
		}
	}

	MoveCameraTo(playerPosition, 2.5f * time);

	Direction previousDirection = travelDirection;

	bool isMoving = false;

	if (Keyboard::isKeyPressed(Keyboard::Key::Up) || Keyboard::isKeyPressed(Keyboard::Key::W))
	{
		Move(0.0f,-60.0f * time);
		travelDirection = Direction::Up;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Down) || Keyboard::isKeyPressed(Keyboard::Key::S))
	{
		Move(0.0f, 60.0f * time);
		travelDirection = Direction::Down;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Right) || Keyboard::isKeyPressed(Keyboard::Key::D))
	{
		Move(60.0f * time, 0.0f);
		travelDirection = Direction::Right;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Left) || Keyboard::isKeyPressed(Keyboard::Key::A))
	{
		Move(-60.0f * time, 0.0f);
		travelDirection = Direction::Left;
		isMoving = true;
	}

	if (orbSpawnTimer <= 0.0f)
	{
		if (Mouse::isButtonPressed(Mouse::Left))
		{
			orbSpawnTimer = ORB_SPAWN_RATE;
			MagicOrb::Fire(GetMap(), GetSprite()->getPosition(), GetMouseWorldCoordinates() - GetSprite()->getPosition());
		}
		else if (Keyboard::isKeyPressed(Keyboard::Key::Space))
		{
			orbSpawnTimer = ORB_SPAWN_RATE;
			MagicOrb::Fire(GetMap(), GetSprite()->getPosition(), travelDirection);
		}
	}
	else
	{
		orbSpawnTimer -= time;
	}

	moving = isMoving;

	if (moving)
	{
		UpdateSprite();
		UpdateAnimations(dt);
	}

	if (Math::SpritesIntersect(*GetSprite(),Door::GetDoor()->GetSprite()))
	{
		reachedDoor = true;
		EnableRendering(false);
	}
}

Player::~Player()
{
	if (currentPlayer == this)
	{
		currentPlayer = nullptr;
	}
}
