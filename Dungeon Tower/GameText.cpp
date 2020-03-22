#include "GameText.h" //Contains the GameText class, which is used for rendering text to the screen
#include "resource.h" //Contains the IDs of the game's resources

ResourceFont GameText::mainGameFont{ GAME_FONT }; //The main game font resource

//Constructs a new GameText object
GameText::GameText(const std::string message, sf::Vector2f position, unsigned int characterSize, sf::Vector2f scale) :
	text(message, mainGameFont.GetFont(), characterSize)
{
	//Set the text's position
	text.setPosition(position);

	//Enable rendering
	EnableRendering();

	//Set the render layer
	SetRenderLayer(100);

	//Set the text's scale
	text.setScale(scale);
}

//Get the text object stored in the GameText object
sf::Text& GameText::GetText()
{
	return text;
}

//Get the text object stored in the GameText object
const sf::Text& GameText::GetText() const
{
	return text;
}

//Renders the GameText object
void GameText::Render(sf::RenderWindow& window)
{
	//Store the old camera view
	auto oldView = window.getView();

	//Store the old text position
	auto oldPosition = text.getPosition();

	//Get the text's scale
	auto scale = text.getScale();

	//Get the local bounds of the text
	auto bounds = text.getLocalBounds();

	//Set the position of the text to be centered
	text.setPosition({-((bounds.width / 2.0f) * scale.x) + oldPosition.x,-((bounds.height / 2.0f) * scale.y) + oldPosition.y});

	//Set the view size to be half the size of the window size
	window.setView(sf::View({ 0.0f,0.0f }, static_cast<sf::Vector2f>(window.getSize()) / 2.0f));

	//Draw the text
	window.draw(text);

	//Set the old view
	window.setView(oldView);

	//Set the text's old position
	text.setPosition(oldPosition);
}
