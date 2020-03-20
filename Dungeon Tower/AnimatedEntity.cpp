#include "AnimatedEntity.h"

void AnimatedEntity::SetAnimatedSprite()
{
	if (spriteGroups.count(currentSpriteGroup) != 0)
	{
		auto& vect = spriteGroups[currentSpriteGroup];

		if (vect.size() > 0)
		{
			SetSprite(&vect[spriteGroupIndex]);
		}
	}
}

AnimatedEntity::AnimatedEntity(const WorldMap& map, bool pingPongAnimation, float animationSpeed, bool enableCollision) :
	Entity(map,enableCollision),
	animationSpeed(animationSpeed),
	PingPong(pingPongAnimation)
{

}



void AnimatedEntity::SetSpriteGroup(std::string spriteGroup)
{
	if (currentSpriteGroup != spriteGroup)
	{
		pingPongDirectionForward = true;
		spriteGroupIndex = 0;
		currentSpriteGroup = spriteGroup;

		SetAnimatedSprite();
	}
}

void AnimatedEntity::AddSprite(std::string group, sf::Sprite sprite)
{
	auto& vector = spriteGroups[group];

	vector.push_back(sprite);
}

void AnimatedEntity::EmplaceSprite(std::string group, sf::Texture& texture, sf::Rect<int> rect,sf::Vector2f scale)
{
	auto& vector = spriteGroups[group];

	sf::Sprite sprite = sf::Sprite(texture, rect);


	//vector.emplace_back(texture,rect);

	sprite.setScale(scale);

	vector.push_back(sprite);

	//vector.push_back(sf::Sprite(std::forward<ParamTypes>(constructorParameters)...));
	//vector.emplace_back(std::forward<ParamTypes>(constructorParameters)...);
}


/*sf::Sprite* AnimatedEntity::GetCurrentSprite()
{
	if (spriteGroups.count(currentSpriteGroup) == 0)
	{
		return nullptr;
	}
	else
	{
		return &spriteGroups[currentSpriteGroup][spriteGroupIndex];
	}
}*/

void AnimatedEntity::UpdateAnimations(sf::Time dt)
{
	if (spriteGroups.count(currentSpriteGroup) == 0)
	{
		return;
	}

	animationTimer += dt.asSeconds();

	if (animationTimer >= animationSpeed)
	{
		animationTimer -= animationSpeed;

		if (PingPong)
		{
			if (pingPongDirectionForward)
			{
				spriteGroupIndex++;
				if (spriteGroupIndex >= spriteGroups[currentSpriteGroup].size() - 1)
				{
					spriteGroupIndex == spriteGroups[currentSpriteGroup].size() - 1;
					pingPongDirectionForward = false;
				}
			}
			else
			{
				spriteGroupIndex--;
				if (spriteGroupIndex <= 0)
				{
					spriteGroupIndex = 0;
					pingPongDirectionForward = true;
				}
			}
		}
		else
		{
			spriteGroupIndex++;
			if (spriteGroupIndex >= spriteGroups[currentSpriteGroup].size())
			{
				spriteGroupIndex = 0;
			}
		}
		SetAnimatedSprite();
	}
}
