#include "HeartDisplay.h"
#include "resource.h"

using namespace std;
using namespace sf;


ResourceTexture HeartDisplay::heartTexture{HEART_SPRITE};


HeartDisplay::HeartDisplay(const Player& player) :
	player(player),
	heartSprite(heartTexture.GetTexture())
{
	SetRenderLayer(100);

	Renderable::EnableRendering();
}

void HeartDisplay::Render(sf::RenderWindow& window)
{
	auto oldView = window.getView();

	auto windowSize = window.getSize();

	auto newView = View({ 0.0f,0.0f }, Vector2f( windowSize.x / 3u,windowSize.y / 3u ));

	window.setView(newView);

	auto center = newView.getCenter();
	auto size = newView.getSize();

	auto viewingRect = Rect<int>(center.x - (size.x / 2), center.y + (size.y / 2), size.x, size.y);

	auto spriteBounds = heartSprite.getLocalBounds();

	auto startPosition = Vector2f(viewingRect.left + viewingRect.width - spriteBounds.width,viewingRect.top - viewingRect.height);

	for (int i = 0; i < player.GetHealth(); i++)
	{
		heartSprite.setPosition(startPosition);
		window.draw(heartSprite);
		startPosition = startPosition - Vector2f(spriteBounds.width,0.0f);
	}

	window.setView(oldView);
}
