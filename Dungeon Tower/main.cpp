#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
//#include "Renderer.h"
//#include "Console.h"
//#include "Common.h"
//#include "Room.h"
//#include "WorldMap.h"
#include "ResourceTexture.h"
#include "resource.h"
#include "Common.h"
#include "WorldMap.h"

using namespace std;



int main()
{
	Common::CreateSprites();
	//Console::SetFullScreenConsole(true);


	//ResourceTexture blankTexture = ResourceTexture(BLANK_TILE);

	//sf::Sprite sprite = sf::Sprite(blankTexture.GetTexture());

	//sprite.setOrigin(8, 8);

	//sprite.setPosition({ 0,0 });

	//sf::RenderWindow window(sf::VideoMode(800, 600), "This is a test");
	auto& window = Common::window;

	//sf::View view = window.getView();

	auto windowSize = window.getSize();

	//view.setCenter({windowSize.x / 2.0f,windowSize.y / 2.0f});

	//view.setRotation(90);

	//window.setView(sf::View({ 0,0 }, {(float)windowSize.x,(float)windowSize.y}));
	//window.setView(view);

	WorldMap map;

	map.Generate(100);

	window.setView(sf::View({ map.GetWidth() * 8.0f, map.GetHeight() * 8.0f }, sf::Vector2f(window.getSize()) / 2.0f));

	sf::Clock clock;
	

	while (window.isOpen())
	{
		sf::Event e;

		auto dt = clock.restart();

		if (window.pollEvent(e))
		{
			if (e.type == e.Closed)
			{
				window.close();
			}

			if (e.type == e.Resized)
			{
				window.setView(sf::View({ map.GetWidth() * 8.0f,map.GetHeight() * 8.0f }, sf::Vector2f(window.getSize()) / 2.0f));
			}
		}

		auto view = window.getView();

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
		{
			view.move(0, -200.0f * dt.asSeconds());
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
		{
			view.move(0, 200.0f * dt.asSeconds());
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
		{
			view.move(200.0f * dt.asSeconds(),0);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
		{
			view.move(-200.0f * dt.asSeconds(), 0);
		}

		window.setView(view);

		//sprite.rotate(90 * dt.asSeconds());

		window.clear(sf::Color::Black);

		//map.Generate(2);

		//window.draw(sprite);

		//map.Render(window);

		map.Render(window);

		//auto& layer = map.GetBackgroundLayer();

		

		

		/*auto& layer = map.GetBackgroundLayer();

		auto size = sf::Vector2i(view.getSize()) / 16;

		auto viewRect = sf::Rect<int>((sf::Vector2i(view.getCenter()) / 16) - sf::Vector2i(size.x / 2, -size.y / 2), sf::Vector2i(view.getSize()) / 16);

		auto viewingArea = sf::Rect<int>(floorf(viewRect.left), ceilf(viewRect.top), ceilf(viewRect.width), ceilf(viewRect.height));

		for (int x = viewingArea.left; x <= (viewingArea.left + viewingArea.width); x++)
		{
			for (int y = (viewingArea.top - viewingArea.height); y <= viewingArea.top; y++)
			{
				//auto coords = window.mapPixelToCoords({ x,y });
				if (x < 0|| x >= layer.GetWidth() || y < 0 || y >= layer.GetHeight())
				{
					break;
				}
				else
				{
					auto tile = layer[{x, y}];
					if (tile != nullptr)
					{
						window.draw(layer[{x, y}]->GetSprite());
					}
				}
			}
		}*/

		/*auto& layer = map.GetBackgroundLayer();

		for (int x = 0; x < layer.GetWidth(); x++)
		{
			for (int y = 0; y < layer.GetHeight(); y++)
			{
				auto tile = layer[{x,y}];
				if (tile != nullptr)
				{
					window.draw(layer[{x, y}]->GetSprite());
				}
			}
		}*/

		//var backgroundLayer = 

		//ResourceFile file = ResourceFile(BLANK_TILE);

		//file.


		window.display();

	}

	return 0;

	//PrintTitleScreen();

	//PrintInstructions();

	//Console::UseUnicode();



	//Console::SetFullScreenConsole(false);

	//system("cls");

	//WorldMap map;

	//map.Generate(2);

	//Renderer::SortDisplayables();

	//Renderer::CameraPosition = map.GetSpawnPoint();

	//while (true)
	//{
		//Console::ClearScreen();
		//Renderer::Render();

		//Renderer::CameraPosition += Vector2(0, 1);
	//}

	/*for (auto& room : map.GetFirstRoom()->GetAllConnectedRooms())
	{
		room->Render();
	}*/

	//Vector2 windowSize = Console::GetConsoleWindowSize();

	//wcout << L"Window Size = " << get<0>(windowSize) << L", " << get<1>(windowSize) << L'\n';

	//Room room = Room({ 20,20 }, { 60,30 });

	//room.Render();

	//WaitForInput();

	//Renderer::CameraPosition = windowSize / -2;

	/*Room room = Room({ get<0>(windowSize) / 2,get<1>(windowSize) / 2 }, { 60,30 });

	for (int x = 0; x < 60; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			auto ptr = room[{x, y}];
			if (ptr != nullptr)
			{
				ptr->Render();
			}
		}
	}

	WaitForInput();*/

	//for (int i = 0; i <= 15; i++)
	//{
		//for (int j = 0; j <= 15; j++)
		//{
			//Console::DrawCharacter(i, i, L'&');
			//Console::SetColorAtPosition(i, i, static_cast<Color>(i), Black);
			//Console::SetColor(static_cast<Color>(i), static_cast<Color>(j));
			//wcout << L"&";
		//}
	//}

	/*int width = get<0>(Console::GetConsoleWindowSize());

	for (int i = 0; i <= 16 * 16; i++)
	{
		WORD Attributes;

		auto position = COORD();

		position.X = i % width;
		position.Y = i / width;

		DWORD charactersRead = 0;

		ReadConsoleOutputAttribute(Console::GetConsoleHandle(), &Attributes, 1, position, &charactersRead);

		Console::SetColor();

		wcout << L"Stats for character " << i << L'\n';

		wcout << boolalpha;

		wcout << L"Foreground Red = " << ((Attributes | FOREGROUND_RED) == FOREGROUND_RED) << "\n";
		wcout << L"Foreground Blue = " << ((Attributes | FOREGROUND_BLUE) == FOREGROUND_BLUE) << "\n";
		wcout << L"Foreground Green = " << ((Attributes | FOREGROUND_GREEN) == FOREGROUND_GREEN) << "\n";
		wcout << L"Foreground Intensity = " << ((Attributes | FOREGROUND_INTENSITY) == FOREGROUND_INTENSITY) << "\n";
		wcout << L"Background Red = " << ((Attributes | BACKGROUND_RED) == BACKGROUND_RED) << "\n";
		wcout << L"Background Blue = " << ((Attributes | BACKGROUND_BLUE) == BACKGROUND_BLUE) << "\n";
		wcout << L"Background Green = " << ((Attributes | BACKGROUND_GREEN) == BACKGROUND_GREEN) << "\n";
		wcout << L"Background Intensity = " << ((Attributes | BACKGROUND_INTENSITY) == BACKGROUND_INTENSITY) << "\n";
		wcout << "End\n\n";

		//Console::GetCharacter(Vector2(i, 0));
	}*/

	/*for (int i = 0; i < get<0>(Console::GetConsoleWindowSize()) - 1; i++)
	{
		Console::DrawCharacter(i, 10 + i, L'&');
	}*/

    //Console::PrintCentered(cout, "This should print centered"s);
	
}