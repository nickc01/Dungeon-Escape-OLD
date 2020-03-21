#include "DialogBoxSprite.h"
#include "resource.h"

using namespace sf;
using namespace std;

ResourceTexture DialogBoxSprite::dialogBoxTexture{ DIALOG_BOX_TEXTURE };

DialogBoxSprite::DialogBoxSprite() :
	dialogBoxSprite(dialogBoxTexture.GetTexture())
{
	auto textureRect = dialogBoxSprite.getTextureRect();

	dialogBoxSprite.setOrigin(textureRect.width / 2.0f, textureRect.height / 2.0f);

	SetRenderLayer(50);

	Renderable::EnableRendering();
}

void DialogBoxSprite::Render(sf::RenderWindow& window)
{

	auto oldView = window.getView();

	auto windowSize = window.getSize();

	auto newView = View({ 0.0f,0.0f }, Vector2f(windowSize.x / 3u, windowSize.y / 3u));

	window.setView(newView);

	window.draw(dialogBoxSprite);

	window.setView(oldView);
}
