#pragma once
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include "Entity.h" //Contains the Entity class
#include <map> //Contains the map class for storing data via keys
#include <string> //Contains std::string
#include <vector> //Contains std::vector for storing objects in an array


//Similar to a normal entity, but allows for animation
class AnimatedEntity : public Entity
{
	float animationSpeed = 1.0f / 12.0f; //How the animation should go. This is the amount of time in between sprites
	float animationTimer = 0.0f; //Keeps track of how much time is left before moving on to the next sprite

	bool PingPong = true; //Whether the animation should go in a back and forth motion or not

	bool pingPongDirectionForward = true; //Stores the current ping pong direction of the animation, whether the animation is going forwards or backwards
	int spriteInGroup = 0; //The current sprite within the group to animate
	std::string currentAnimationGroup; //The current group of sprites to animate with
	std::map<std::string, std::vector<sf::Sprite>> animationGroups; //The list of all sprite groups. Each group represents a series of sprites that are part of an animation

	//Updates the current sprite
	void UpdateAnimatedSprite();

public:

	//Constructs an animated entity
	AnimatedEntity(const WorldMap& map, bool doPingPong = true, float animationSpeed = 1.0f / 12.0f, bool enableCollision = true);

	//Runs the specified animation sprite group if one is currently set
	void UpdateAnimations(sf::Time dt);

	//Sets the current animation group to animate with
	void SetAnimationGroup(std::string animationGroup);

	//Adds an existing sprite to an animation group
	void AddSprite(std::string group,sf::Sprite sprite);

	//Constructs a new sprite and adds it to an animation group
	void EmplaceSprite(std::string group, sf::Texture& texture, sf::Rect<int> rect, sf::Vector2f scale = sf::Vector2f(1.0f, 1.0f));

	//The update function for the entity
	virtual void Update(sf::Time dt) override = 0;
};

