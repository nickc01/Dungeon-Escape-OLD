#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <memory> //Contains std::shared_ptr and std::unique_ptr

//A sprite that represents a background portion of the worldmap
class BackgroundTile
{
	//The sprite of the background tile
	sf::Sprite sprite;

	//Whether the tile is collidable or not
	bool collidable;

	//Constructs a new background tile with the specified sprite
	BackgroundTile(const sf::Sprite& sprite, bool collidable = false);

	//Constructs a new background tile with the specified sprite and position
	BackgroundTile(const sf::Sprite& sprite, sf::Vector2<int> position, bool collidable = false);

public:

	//Returns whether the sprite is colidable or not
	bool IsCollidable() const;


	//Gets the sprite of the tile
	const sf::Sprite& GetSprite() const;

	//Gets the sprite of the tile
	sf::Sprite& GetSprite();

	//Sets the sprite of the tile
	void SetSprite(const sf::Sprite& sprite);

	//Create a new shared_ptr of a background tile
	static std::shared_ptr<BackgroundTile> Create(const sf::Sprite& sprite, bool collidable = false);

	//Create a new shared_ptr of a background tile
	static std::shared_ptr<BackgroundTile> Create(const sf::Sprite& sprite, sf::Vector2<int> position, bool collidable = false);
};

