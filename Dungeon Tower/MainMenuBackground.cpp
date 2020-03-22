#include "MainMenuBackground.h" //Contains the MainMenuBackground class, which displays the background you see on the main menu
#include "resource.h" //Contains the IDs of the game's resources

ResourceTexture MainMenuBackground::menuBackgroundTexture{BACKGROUND_TILES}; //The texture resource for the main menu background

//Renders the menu background to the screen
void MainMenuBackground::Render(sf::RenderWindow& window)
{
	//Store the old view
	auto oldView = window.getView();

	//Set the new view size to be half the window's size
	window.setView(sf::View({0.0f,0.0f},static_cast<sf::Vector2f>(window.getSize()) / 2.0f));

	//Draw the main men
	window.draw(menuBackgroundSprite);

	//Set the old view
	window.setView(oldView);
}

//Constructs a new MainMenuBackground
MainMenuBackground::MainMenuBackground()
{
	//Set the texture to be tiled
	menuBackgroundTexture.GetTexture().setRepeated(true);

	//Create a new sprite with the background texture
	menuBackgroundSprite = sf::Sprite(menuBackgroundTexture.GetTexture());

	//Get the texture's rect
	auto textureRect = menuBackgroundSprite.getTextureRect();

	//Multiply the dimensions by 200
	textureRect.width *= 200;
	textureRect.height *= 200;

	//Center the sprite's position in the texture
	sf::Vector2f position = sf::Vector2f(-textureRect.width / 2.0f,-textureRect.height / 2.0f );

	//Set the sprite's new position
	menuBackgroundSprite.setPosition(position);

	//Set the new texture rect for the sprite
	menuBackgroundSprite.setTextureRect(textureRect);

	//Enable rendering
	EnableRendering();
}
