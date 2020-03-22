#pragma once
#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include "ResourceFont.h" //Contains the ResourceFont class, which is used to load font resources


class GameText : public Renderable
{
	static ResourceFont mainGameFont; //The main game font resource

	sf::Text text; //The text object

public:

	//Constructs a new GameText object
	GameText(const std::string text, sf::Vector2f position, unsigned int characterSize = 300u, sf::Vector2f scale = sf::Vector2f(0.1f,0.1f));

	//Get the text object stored in the GameText object
	sf::Text& GetText();
	//Get the text object stored in the GameText object
	const sf::Text& GetText() const;


	//Renders the GameText object
	virtual void Render(sf::RenderWindow& window) override;

};

