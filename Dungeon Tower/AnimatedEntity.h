#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include <map>
#include <string>
#include <vector>

class AnimatedEntity : public Entity
{
	float animationSpeed;
	float animationTimer = 0.0f;

	bool PingPong = true;

	bool pingPongDirectionForward = true;
	int spriteGroupIndex = 0;
	std::string currentSpriteGroup;
	std::map<std::string, std::vector<sf::Sprite>> spriteGroups;

	void SetAnimatedSprite();

public:

	AnimatedEntity(const WorldMap& map, bool pingPongAnimation = true, float animationSpeed = 1.0f / 12.0f, bool enableCollision = true);

	void UpdateAnimations(sf::Time dt);
	void SetSpriteGroup(std::string spriteGroup);

	void AddSprite(std::string group,sf::Sprite sprite);

	void EmplaceSprite(std::string group, sf::Texture& texture, sf::Rect<int> rect, sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f));

	//sf::Sprite* GetCurrentSprite();

	virtual void Update(sf::Time dt) override = 0;
};

