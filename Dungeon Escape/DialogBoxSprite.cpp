#include "DialogBoxSprite.h" //Contains the DialogBoxSprite class, which is used to display a dialog box to the screen
#include "resource.h" //Contains the IDs of the game's resources

using namespace sf; //Prevents me from having to type sf everywhere
using namespace std; //Prevents me from having to type std everywhere

//Loads the resource texture
ResourceTexture DialogBoxSprite::dialogBoxTexture{ DIALOG_BOX_TEXTURE };

//Constructs a new dialog box
DialogBoxSprite::DialogBoxSprite() :
	//Creates the dialog box sprite with the loaded texture
	dialogBoxSprite(dialogBoxTexture.GetTexture())
{
	//Gets the texture rect of the sprite
	auto textureRect = dialogBoxSprite.getTextureRect();

	//Set the origin to the middle of the sprite
	dialogBoxSprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	//Update the layer the sprite is rendering on
	SetRenderLayer(50);

	//Enable rendering for this object
	Renderable::EnableRendering();
}

//Called when the dialog box is to be rendered
void DialogBoxSprite::Render(sf::RenderWindow& window)
{
	//Store the old view of the window
	auto oldView = window.getView();

	//Get the window's current size
	auto windowSize = window.getSize();

	//Set a new view that is centered at (0,0) and the size to be 1/3 of the window's size
	auto newView = View({ 0.0f,0.0f }, Vector2f(windowSize.x / 3u, windowSize.y / 3u));

	//Update the windows's view
	window.setView(newView);

	//Draw the dialog box sprite
	window.draw(dialogBoxSprite);

	//Set the old view
	window.setView(oldView);
}
