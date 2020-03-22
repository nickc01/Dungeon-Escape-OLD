#include "GameManager.h" //Contains the GameManager class, which contains the main logic of the game
#include "Common.h" //Contains many common game functions and variables
#include "ThreadPool.h" //Contains the thread pool for running code asynchronously

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

//The Update function that is called when the GameState is set to MainMenu
void GameManager::MenuUpdate()
{
	//If the enter key has been pressed
	if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		//Destroy the title text
		TitleText = nullptr;

		//The enter key is currently being held
		enterHeld = true;

		//Go to the story screen
		currentState = GameState::StoryScreen;

		//The mains tory text
		std::string storyText = "Oh no!\n\nYou have been trapped\ninside of a large, trecherous\ndungeon.\n\nA dungeon that is of course,\nfull of skeletons!\n\nWill you be able to find a way out\nwithout getting torn to bits?";

		//Create the story text
		StoryText = make_unique<GameText>(storyText, Vector2f(0, -50), 150);
	}
}

//The Update function that is called when the GameState is set to StoryScreen
void GameManager::StoryUpdate()
{
	//If enter is being held
	if (enterHeld)
	{
		//Wait until it is released
		if (!Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			enterHeld = false;
		}
	}
	//If enter is not being held down
	else
	{
		//Wait until the enter key is pressed again
		if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			//Enter is now being held down
			enterHeld = true;

			//Go to the instructions screen
			currentState = GameState::InstructionScreen;

			//Destroy the story text
			StoryText = nullptr;

			//The instructional text
			std::string instructionText = "Instructions\n\nUse Arrow Keys or WASD\nto move around\n\nUse spacebar or left mouse button to shoot magic\n\nAvoid the skeletons\n\nFind the exit door to win\n\n";

			//Create the instructional text
			InstructionText = make_unique<GameText>(instructionText,Vector2f(0,-50),100);
		}
	}
}

//The Update function that is called when the GameState is set to InstructionScreen
void GameManager::InstructionUpdate()
{

	//If enter is being held
	if (enterHeld)
	{
		//Wait until it is released
		if (!Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			enterHeld = false;
		}
	}
	//If enter is not being held down
	else
	{
		//Wait until the enter key is pressed again
		if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			//Go to the game state
			currentState = GameState::Game;

			//Destroy the instructional text
			InstructionText = nullptr;
			//Destroy the enter text
			EnterText = nullptr;
			//Destroy the menu background
			MenuBackground = nullptr;

			//Create the world map object
			map = make_unique<WorldMap>();

			//Start generating the world
			map->GenerateAsync(20);
		}
	}
}

//The Update function that is called when the GameState is set to Game
void GameManager::GameUpdate()
{
	//If the world has not been generated yet
	if (generated == false)
	{
		//If the generation progress is complete
		if (map->GetProgress() == 1.0f)
		{
			//The world has been generated
			generated = true;
			AfterGeneration();
		}
		//If the progress is not complete yet
		else
		{
			//Break out of the function
			return;
		}
	}

	//If the player no longer alive
	if (!player->IsAlive())
	{
		//If the player has reached the door, then the player has won
		if (player->ReachedTheDoor())
		{
			//Win the game
			OnWin();
			return;
		}
		//If the player has not reached the door, then the player has lost
		else
		{
			//Lose the game
			OnLose();
			return;
		}
	}
	//If the GameManager wants to end the game
	if (endingGame)
	{
		//End the game
		End();
		return;
	}
}

//The Update function that is called when the GameState is set to Win or Lose
void GameManager::EndUpdate()
{
	//If the enter key is pressed
	if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		//End the game
		End();
	}
}

//Called when the player wins the game
void GameManager::OnWin()
{
	//Set the game state to win
	currentState = GameState::Win;

	//Create the dialog box that says "You Win!"
	OnGameEnd("You Win!");
}

//Called when the player looses the game
void GameManager::OnLose()
{
	//Set the game state to lose
	currentState = GameState::Lose;

	//Create the dialog box that says "You Lose!"
	OnGameEnd("You Lose!");
}

//Called either when the player wins or looses
void GameManager::OnGameEnd(std::string titleText)
{
	//Create the dialog box
	dialogBox = make_unique<DialogBoxSprite>();

	//Create the dialog box title text
	dialogTitleText = make_unique<GameText>(titleText, Vector2f(0, -50), 200);

	//Create the dialog box enter text
	dialogEnterText = make_unique<GameText>("Press enter to Exit",Vector2f(0,50),100);
}

//Constructs a new GameManager
GameManager::GameManager() : UpdateReceiver(true) {}

//Ends the game
void GameManager::EndTheGame()
{
	//Tell the game manager to end the game
	endingGame = true;
}

//Called to start the GameManager
void GameManager::Start()
{
	//Star the thread pool for running code asynchronously
	ThreadPool::Start();


	//Create the main menu background
	MenuBackground = make_unique<MainMenuBackground>();
	//Create the title text
	TitleText = make_unique<GameText>("Dungeon Escape",Vector2f(0,-100),300);
	//Create the enter text
	EnterText = make_unique<GameText>("Press Enter to Continue",Vector2f(0,100),150);
}

//Called after the world has been generated
void GameManager::AfterGeneration()
{
	//Center the camera to be in the middle of the map
	Common::CenterCamera(Vector2f(map->GetWidth() * 8.0f, map->GetHeight() * 8.0f));

	//Create the player
	player = make_unique<Player>(*map, static_cast<Vector2f>(map->GetSpawnPoint()));

	//Create the heart display
	heartDisplay = make_unique<HeartDisplay>(*player);

	//Create all of the skeletons
	for (auto& enemySpawnPoint : map->GetEnemySpawnPoints())
	{
		skeletons.emplace_back(*map, enemySpawnPoint);
	}

	//Create the exit door
	door = make_unique<Door>(map->GetDoorLocation());
}

//Whether the Game Manager is completed or not
bool GameManager::IsComplete() const
{
	return isComplete;
}

//The update function that is called each frame
void GameManager::Update(sf::Time dt)
{
	switch (currentState)
	{
		//If the game state is MainMenu
	case GameState::MainMenu:
		MenuUpdate(); //Call the main menu update function
		break;
		//If the game state is StoryScreen
	case GameState::StoryScreen:
		StoryUpdate(); //Call the story screen update function
		break;
		//If the game state is InstructionScreen
	case GameState::InstructionScreen:
		InstructionUpdate(); //Call the instruction screen update function
		break;
		//If the game state is Game
	case GameState::Game:
		GameUpdate(); //Call the game update function
		break;
		//If the game state is Win
	case GameState::Win:
		EndUpdate(); //Call the End update function
		break;
		//If the game state is Lose
	case GameState::Lose:
		EndUpdate(); //Call the End update function
		break;
	}
	//If the game is to be ended
	if (endingGame)
	{
		//End the game
		End();
	}
}

//Called to end the Game Manager
void GameManager::End()
{
	//Set complete to true
	isComplete = true;

	//Stop the thread pool
	ThreadPool::Stop();
}
