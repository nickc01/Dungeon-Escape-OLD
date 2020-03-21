#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include "ResourceTexture.h"
#include "Common.h"
#include "WorldMap.h"
#include "UpdateReceiver.h"
#include <mutex>
#include "GameManager.h"
#include <thread>

using namespace std;
using namespace sf;

//void UpdateWindowView(Vector2f Center);

int main()
{
	Common::CreateSprites();
	auto& window = Common::MainWindow;

	//auto windowSize = window.getSize();

	//window.view

	//WorldMap map;

	//auto mapValueAddress = &map;

	//map.Generate(20);

	RefreshWindow();

	//Common::MainWindow.setView(View({ map.GetWidth() * 8.0f, map.GetHeight() * 8.0f }, Vector2f(Common::MainWindow.getSize()) / 2.0f));

	//CenterCamera(map.GetDimensions());

	//Vector2f mapCenter = { map.GetWidth() * 8.0f, map.GetHeight() * 8.0f };

	//UpdateWindowView(mapCenter);

	//thread gameThread(GameManager::StartGame);

	GameManager manager{};

	manager.Start();

	Clock clock;

	while (!manager.IsComplete())
	{
		Event e;

		auto dt = clock.restart();
		{
			auto lockObject = unique_lock<recursive_mutex>(UpdateReceiver::GetMutex());
			auto updatables = UpdateReceiver::GetUpdatables();

			for (auto i = updatables.rbegin(); i != updatables.rend(); i++)
			{
				(**i).Update(dt);
			}
		}


		if (window.pollEvent(e))
		{
			if (e.type == e.Closed)
			{
				window.close();
				manager.EndTheGame();
			}

			if (e.type == e.Resized)
			{
				RefreshWindow();
			}
		}


		window.clear(Color::Black);

		{
			auto lockObject = unique_lock<recursive_mutex>(Renderable::GetMutex());

			auto& renderables = Renderable::GetRenderables();

			for (auto i = rbegin(renderables); i != rend(renderables); i++)
			{
				(**i).Render(window);
			}
		}

		window.display();

		if (manager.IsComplete())
		{
			window.close();
		}

	}

	manager.EndTheGame();
	//GameManager::EndingGame = true;
	//gameThread.join();
}
