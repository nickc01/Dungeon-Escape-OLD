/*
	Nicholas Cullen
	3/22/20
	Dungeon Escape : A small GUI C++ game where you attempt to escape a dungeon full of skeletons
*/
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <string> //Contains std::string
#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources
#include "Common.h" //Contains many common game functions and variables
#include "WorldMap.h" //Contains the WorldMap class
#include "UpdateReceiver.h" //Contains the UpdateReceiver class for receiving update events
#include <mutex> //Contains std::mutex and std::recursive_mutex for resource locking and to prevent data races //Contains std::mutex and std::recursive_mutex for mutual exclusion of resources and prevents data races
#include "GameManager.h" //Contains the GameManager class, which contains the main logic of the game

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere


//The start of the program
int main()
{
	//Create all the common sprites
	Common::CreateSprites();

	Common::MainWindow.setVerticalSyncEnabled(true);

	//Refresh the window size
	Common::RefreshWindow();

	//Create the game manager
	GameManager manager{};

	//Start the game manager
	manager.Start();

	//Start the time clock
	Clock clock;

	//Repeat until the game manager is complete
	while (!manager.IsComplete())
	{
		//The current window event
		Event e;

		//Reset the clock and get the time between the last frame and the current frame
		auto dt = clock.restart();


		{
			//Lock the updatables list lock
			auto lockObject = unique_lock<recursive_mutex>(UpdateReceiver::GetMutex());
			//Get the updatable objects
			auto updatables = UpdateReceiver::GetUpdatables();

			//Loop over all the updatable objects
			for (auto i = updatables.rbegin(); i != updatables.rend(); i++)
			{
				//Call their update function
				(**i).Update(dt);
			}
		}

		//Poll the window for events
		if (Common::MainWindow.pollEvent(e))
		{
			//If the event is to close the window
			if (e.type == e.Closed)
			{
				//Close the window
				Common::MainWindow.close();
				//Tell the manager to end the game
				manager.EndTheGame();
			}

			//if the event is to resize the window
			if (e.type == e.Resized)
			{
				//Refresh the window size
				Common::RefreshWindow();
			}
		}

		//Clear all the window's contents
		Common::MainWindow.clear(Color::Black);

		{
			//Lock the renderable object list
			auto lockObject = unique_lock<recursive_mutex>(Renderable::GetMutex());

			//Get the list of renderable objects
			auto& renderables = Renderable::GetRenderables();

			//Loop over all the renderable objects
			for (auto i = rbegin(renderables); i != rend(renderables); i++)
			{
				//Render each of the objects to the main window
				(**i).Render(Common::MainWindow);
			}
		}

		//Display the window's contents to the screen
		Common::MainWindow.display();

		//if the game manager is complete
		if (manager.IsComplete())
		{
			//Close the main window
			Common::MainWindow.close();
		}

	}

	//End the game
	manager.EndTheGame();
}
