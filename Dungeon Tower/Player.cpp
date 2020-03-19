#include "Player.h"
#include "resource.h"
#include "Common.h"
#include "Math.h"


using namespace std;
using namespace sf;


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

const Player* Player::GetCurrentPlayer()
{
	return currentPlayer;
}

Player::Player(const WorldMap& map, sf::Vector2f spawnPoint) :
	AnimatedEntity(map,true,1.0f / 12.0f,false,false)
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

void Player::Update(sf::Time dt)
{
	auto time = dt.asSeconds();

	auto playerPosition = GetSprite()->getPosition();

	MoveCameraTo(playerPosition, 2.5f * time);

	Direction previousDirection = travelDirection;

	bool isMoving = false;

	if (Keyboard::isKeyPressed(Keyboard::Key::Up))
	{
		Move(0.0f,-64.0f * time);
		travelDirection = Direction::Up;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Down))
	{
		Move(0.0f, 64.0f * time);
		travelDirection = Direction::Down;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Right))
	{
		Move(64.0f * time, 0.0f);
		travelDirection = Direction::Right;
		isMoving = true;
	}

	if (Keyboard::isKeyPressed(Keyboard::Key::Left))
	{
		Move(-64.0f * time, 0.0f);
		travelDirection = Direction::Left;
		isMoving = true;
	}

	moving = isMoving;

	if (moving)
	{
		UpdateSprite();
		UpdateAnimations(dt);
	}
}

Player::~Player()
{
	if (currentPlayer == this)
	{
		currentPlayer = nullptr;
	}
}
