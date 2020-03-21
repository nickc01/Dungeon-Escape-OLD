#pragma once
#include "Renderable.h"
#include "ResourceFont.h"

class GameText : public Renderable
{
	static ResourceFont mainGameFont;

	sf::Text text;

public:

	GameText(const std::string text, sf::Vector2f position, unsigned int characterSize = 300u, sf::Vector2f scale = sf::Vector2f(0.1f,0.1f));


	sf::Text& GetText();
	const sf::Text& GetText() const;


	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;

};

