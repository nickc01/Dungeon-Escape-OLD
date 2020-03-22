#pragma once

#include <memory> //Contains std::shared_ptr and std::unique_ptr
#include "UpdateReceiver.h" //Contains the UpdateReceiver class for receiving update events
#include "WorldMap.h" //Contains the WorldMap class
#include "Player.h" //Contains the Player class that represents the player
#include "HeartDisplay.h" //Contains the HeartDisplay class for displaying the amount of hearts the player has 
#include "SkeletonEnemy.h" //Contains the SkeletonEnemy class
#include <list> //Contains std::list for storing items in a linked list
#include "Door.h" //Contains the Door class, which is where the player enters to win the game
#include "MainMenuBackground.h" //Contains the MainMenuBackground class, which displays the background you see on the main menu
#include "GameText.h" //Contains the GameText class, which is used for rendering text to the screen
#include "DialogBoxSprite.h" //Contains the DialogBoxSprite class, which is used to display a dialog box to the screen


//An enum representing the current game state
enum class GameState
{
	MainMenu = 0,
	StoryScreen,
	InstructionScreen,
	Game,
	Win,
	Lose
};

//Manages how the game is run
class GameManager : public UpdateReceiver
{
	//The current state the game is in
	GameState currentState = GameState::MainMenu;

	bool isComplete = false; //Whether the game is complete or not
	bool endingGame = false; //Whether the game should be ending or not

	bool enterHeld = false; //Whether the enter key has been held down or not

	//GAME VARIABLES
	std::unique_ptr<WorldMap> map{}; //The world map
	std::unique_ptr<Player> player; //The player in the world
	std::unique_ptr<HeartDisplay> heartDisplay; //The heart display
	std::unique_ptr<Door> door; //The exit door
	std::list<SkeletonEnemy> skeletons{}; //Enemy Skeletons that are strewn around the map
	bool generated = false; //Whether the map has been fully generated or not

	//MAIN MENU VARIABLES
	std::unique_ptr<MainMenuBackground> MenuBackground; //The main menu background
	std::unique_ptr<GameText> TitleText; //The title text
	std::unique_ptr<GameText> EnterText; //The "Press enter to continue" text

	//STORY VARIABLES
	std::unique_ptr<GameText> StoryText; //The text that contains the story

	//INSTRUCTION VARIABLES
	std::unique_ptr<GameText> InstructionText; //The text that contains the instructions on how to play

	//WIN AND LOSE VARIABLES
	std::unique_ptr<DialogBoxSprite> dialogBox; //The dialog box
	std::unique_ptr<GameText> dialogTitleText; //The dialog box title text
	std::unique_ptr<GameText> dialogEnterText; //The dialog box "Press Enter to continue" text

	void MenuUpdate(); //The Update function that is called when the GameState is set to MainMenu
	void StoryUpdate(); //The Update function that is called when the GameState is set to StoryScreen
	void InstructionUpdate(); //The Update function that is called when the GameState is set to InstructionScreen
	void GameUpdate(); //The Update function that is called when the GameState is set to Game

	void EndUpdate(); //The Update function that is called when the GameState is set to Win or Lose

	void OnWin(); //Called when the player wins the game
	void OnLose(); //Called when the player looses the game
	void OnGameEnd(std::string titleText); //Called either when the player wins or looses

public:

	//Constructs a new GameManager
	GameManager();

	//Deleting these four functions prevents the object from being copied or moved
	GameManager(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	GameManager& operator=(GameManager&&) = delete;

	//Ends the game
	void EndTheGame();

	//Called to start the GameManager
	void Start();

	//Called after the world has been generated
	void AfterGeneration();

	//Whether the Game Manager is completed or not
	bool IsComplete() const;

	//The update function that is called each frame
	virtual void Update(sf::Time dt) override;
	//Called to end the Game Manager
	void End();
};

