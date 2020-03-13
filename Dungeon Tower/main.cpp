#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "ResourceTexture.h"
#include "Common.h"
#include "WorldMap.h"

using namespace std;
using namespace sf;



int main()
{
	Common::CreateSprites();
	auto& window = Common::window;

	auto windowSize = window.getSize();

	WorldMap map;

	map.Generate(2);

	window.setView(View({ map.GetWidth() * 8.0f, map.GetHeight() * 8.0f }, Vector2f(window.getSize()) / 2.0f));

	Clock clock;
	

	while (window.isOpen())
	{
		Event e;

		auto dt = clock.restart();

		if (window.pollEvent(e))
		{
			if (e.type == e.Closed)
			{
				window.close();
			}

			if (e.type == e.Resized)
			{
				window.setView(View({ map.GetWidth() * 8.0f,map.GetHeight() * 8.0f }, Vector2f(window.getSize()) / 2.0f));
			}
		}

		auto view = window.getView();

		if (Keyboard::isKeyPressed(Keyboard::Key::Up))
		{
			view.move(0, -200.0f * dt.asSeconds());
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Down))
		{
			view.move(0, 200.0f * dt.asSeconds());
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Right))
		{
			view.move(200.0f * dt.asSeconds(),0);
		}

		if (Keyboard::isKeyPressed(Keyboard::Key::Left))
		{
			view.move(-200.0f * dt.asSeconds(), 0);
		}

		window.setView(view);

		window.clear(Color::Black);

		map.Render(window);

		window.display();

	}
}