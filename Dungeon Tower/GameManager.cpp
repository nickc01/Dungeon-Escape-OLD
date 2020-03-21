#include "GameManager.h"
#include "Common.h"
#include <thread>
#include "ThreadPool.h"

using namespace std;
using namespace sf;


void GameManager::MenuUpdate()
{
	if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		TitleText = nullptr;


		spaceHeld = true;
		currentState = GameState::StoryScreen;

		std::string storyText = "Oh no!\n\nYou have been trapped\ninside of a large, trecherous\ndungeon.\n\nA dungeon that is of course,\nfull of skeletons!\n\nWill you be able to find a way out\nwithout getting torn to bits?";

		StoryText = make_unique<GameText>(storyText, Vector2f(0, -50), 150);
		//StoryText->GetText().setScale(0.1,0.1f);
	}
}

void GameManager::StoryUpdate()
{
	if (spaceHeld)
	{
		if (!Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			spaceHeld = false;
		}
	}
	else
	{
		if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			spaceHeld = true;
			currentState = GameState::InstructionScreen;

			StoryText = nullptr;

			std::string instructionText = "Instructions\n\nUse Arrow Keys or WASD\nto move around\n\nUse spacebar or left mouse button to shoot magic\n\nAvoid the skeletons\n\nFind the exit door to win\n\n";

			InstructionText = make_unique<GameText>(instructionText,Vector2f(0,-50),100);
		}
	}
}

void GameManager::InstructionUpdate()
{
	if (spaceHeld)
	{
		if (!Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			spaceHeld = false;
		}
	}
	else
	{
		if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
		{
			currentState = GameState::Game;

			InstructionText = nullptr;
			EnterText = nullptr;
			MenuBackground = nullptr;

			map = make_unique <WorldMap>();

			map->GenerateAsync(20);
		}
	}
}

void GameManager::GameUpdate()
{
	if (generated == false)
	{
		if (map->GetProgress() == 1.0f)
		{
			generated = true;
			AfterGeneration();
		}
		else
		{
			return;
		}
	}

	if (!player->IsAlive())
	{
		if (player->ReachedTheDoor())
		{
			OnWin();
			return;
		}
		else
		{
			OnLose();
			return;
		}
	}
	if (endingGame)
	{
		End();
		return;
	}
}

void GameManager::EndUpdate()
{
	if (Keyboard::isKeyPressed(sf::Keyboard::Enter))
	{
		End();
	}
}

void GameManager::OnWin()
{
	currentState = GameState::Win;

	OnGameEnd("You Win!");
}

void GameManager::OnLose()
{
	currentState = GameState::Lose;

	OnGameEnd("You Lose!");
}

void GameManager::OnGameEnd(std::string titleText)
{
	dialogBox = make_unique<DialogBoxSprite>();

	dialogTitleText = make_unique<GameText>(titleText, Vector2f(0, -50), 200);

	dialogEnterText = make_unique<GameText>("Press enter to Exit",Vector2f(0,50),100);
}

GameManager::GameManager() : UpdateReceiver(true) {}

void GameManager::EndTheGame()
{
	endingGame = true;
}

void GameManager::Start()
{
	ThreadPool::Start();


	MenuBackground = make_unique<MainMenuBackground>();
	TitleText = make_unique<GameText>("Dungeon Escape",Vector2f(0,-100),300);
	EnterText = make_unique<GameText>("Press Enter to Continue",Vector2f(0,100),150);
}

void GameManager::AfterGeneration()
{
	CenterCamera(Vector2f(map->GetWidth() * 8.0f, map->GetHeight() * 8.0f));

	player = make_unique<Player>(*map, static_cast<Vector2f>(map->GetSpawnPoint()));

	heartDisplay = make_unique<HeartDisplay>(*player);

	for (auto& enemySpawnPoint : map->GetEnemySpawnPoints())
	{
		skeletons.emplace_back(*map, enemySpawnPoint);
	}

	door = make_unique<Door>(map->GetDoorLocation());
}

bool GameManager::IsComplete() const
{
	return isComplete;
}

void GameManager::Update(sf::Time dt)
{
	switch (currentState)
	{
	case GameState::MainMenu:
		MenuUpdate();
		break;
	case GameState::StoryScreen:
		StoryUpdate();
		break;
	case GameState::InstructionScreen:
		InstructionUpdate();
		break;
	case GameState::Game:
		GameUpdate();
		break;
	case GameState::Win:
		EndUpdate();
		break;
	case GameState::Lose:
		EndUpdate();
		break;
	default:
		break;
	}
	if (endingGame)
	{
		End();
		return;
	}
}

void GameManager::End()
{
	isComplete = true;
	ThreadPool::Stop();
}
