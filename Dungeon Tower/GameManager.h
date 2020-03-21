#pragma once

#include <memory>
#include "UpdateReceiver.h"
#include "WorldMap.h"
#include "Player.h"
#include "HeartDisplay.h"
#include "SkeletonEnemy.h"
#include <list>
#include "Door.h"
#include "MainMenuBackground.h"
#include "GameText.h"
#include "DialogBoxSprite.h"


enum class GameState
{
	MainMenu = 0,
	StoryScreen,
	InstructionScreen,
	Game,
	Win,
	Lose
};

class GameManager : public UpdateReceiver
{
	GameState currentState = GameState::MainMenu;

	bool isComplete = false;
	bool endingGame = false;

	bool spaceHeld = false;

	//GAME VARIABLES
	std::unique_ptr<WorldMap> map{};
	std::unique_ptr<Player> player;
	std::unique_ptr<HeartDisplay> heartDisplay;
	std::unique_ptr<Door> door;
	std::list<SkeletonEnemy> skeletons{};
	bool generated = false;

	//MAIN MENU VARIABLES
	std::unique_ptr<MainMenuBackground> MenuBackground;
	std::unique_ptr<GameText> TitleText;
	std::unique_ptr<GameText> EnterText;

	//STORY VARIABLES
	std::unique_ptr<GameText> StoryText;

	//INSTRUCTION VARIABLES
	std::unique_ptr<GameText> InstructionText;

	//WIN AND LOSE VARIABLES
	std::unique_ptr<DialogBoxSprite> dialogBox;
	std::unique_ptr<GameText> dialogTitleText;
	std::unique_ptr<GameText> dialogEnterText;

	void MenuUpdate();
	void StoryUpdate();
	void InstructionUpdate();
	void GameUpdate();

	void EndUpdate();

	void OnWin();
	void OnLose();
	void OnGameEnd(std::string titleText);

public:

	GameManager();

	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;

	GameManager& operator=(const GameManager&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	void EndTheGame();

	void Start();

	void AfterGeneration();

	bool IsComplete() const;

	// Inherited via UpdateReceiver
	virtual void Update(sf::Time dt) override;

	void End();
};

/*namespace GameManager
{
	extern std::atomic<bool> EndingGame;

	void StartGame();
}*/

