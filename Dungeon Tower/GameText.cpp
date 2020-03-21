#include "GameText.h"
#include "resource.h"

ResourceFont GameText::mainGameFont{ GAME_FONT };

GameText::GameText(const std::string message, sf::Vector2f position, unsigned int characterSize, sf::Vector2f scale) :
	text(message, mainGameFont.GetFont(), characterSize)
{
	text.setPosition(position);

	EnableRendering();

	SetRenderLayer(100);

	text.setScale(scale);
}

sf::Text& GameText::GetText()
{
	return text;
}

const sf::Text& GameText::GetText() const
{
	return text;
}

void GameText::Render(sf::RenderWindow& window)
{
	auto oldView = window.getView();

	auto oldPosition = text.getPosition();

	auto scale = text.getScale();

	auto bounds = text.getLocalBounds();

	text.setPosition({-((bounds.width / 2.0f) * scale.x) + oldPosition.x,-((bounds.height / 2.0f) * scale.y) + oldPosition.y});

	window.setView(sf::View({ 0.0f,0.0f }, static_cast<sf::Vector2f>(window.getSize()) / 2.0f));

	window.draw(text);

	window.setView(oldView);

	text.setPosition(oldPosition);
}
