#include "MainMenuBackground.h"
#include "resource.h"

ResourceTexture MainMenuBackground::backgroundTilesTexture{BACKGROUND_TILES};

void MainMenuBackground::Render(sf::RenderWindow& window)
{
	auto oldView = window.getView();

	window.setView(sf::View({0.0f,0.0f},static_cast<sf::Vector2f>(window.getSize()) / 2.0f));

	window.draw(backgroundSprite);

	window.setView(oldView);
}

MainMenuBackground::MainMenuBackground()
{
	backgroundTilesTexture.GetTexture().setRepeated(true);
	backgroundSprite = sf::Sprite(backgroundTilesTexture.GetTexture());

	auto textureRect = backgroundSprite.getTextureRect();

	textureRect.width *= 200;
	textureRect.height *= 200;

	sf::Vector2f position = sf::Vector2f(-textureRect.width / 2.0f,-textureRect.height / 2.0f );

	backgroundSprite.setPosition(position);

	backgroundSprite.setTextureRect(textureRect);

	EnableRendering();
}
