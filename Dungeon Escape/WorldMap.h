#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <thread> //Contains std::thread for running code on a seperate thread
#include <memory> //Contains std::shared_ptr and std::unique_ptr
#include <vector> //Contains std::vector for storing objects in an array
#include "Room.h" //Contains the Room class that represents individual rooms in the game
#include "Renderable.h" //Contains the Renderable class for rendering to the game window
#include <atomic> //Contains std::atomic, which is used to prevent data races over an entire object


//Represents the world map the player plays in
class WorldMap : public Renderable
{
	std::atomic<float> progress; //How far along the world generation progress is
	std::shared_ptr<Room> TopRoom; //The first room in the entire room hierarchy

	std::unique_ptr<std::thread> thread = nullptr; //The thread that generates the room asynchronously

	std::vector<sf::Vector2f> enemySpawnPoints{}; //All the potential spots an enemy can spawn

	sf::Vector2f doorLocation; //The location of the exit door

	Array2D<std::shared_ptr<BackgroundTile>> tiles; //The list of all tiles that make up the room

	sf::Vector2u tileSize; //The size of each tile in the map

	sf::Vector2<int> SpawnPoint; //The spawnpoint for the player

	int level = 0; //The level of the room. Used to determine difficulty

	//Takes the room and branch hierachy, and flattens it into a 2D grid of tiles
	void Flatten();

public:

	//Constructs an empty world map
	WorldMap();

	//Retrieves the progress of the world generation
	const std::atomic<float>& GetProgress() const;

	//Generates the world map on a seperate thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1.0f, the map is done generating
	void GenerateAsync(int level);

	//Generate the room hierarchy
	void Generate(int level);

	//Gets the width of the world map
	int GetWidth() const;
	//Gets the height of the world map
	int GetHeight() const;

	//Gets the size of each tile in the map
	sf::Vector2u GetTileSize() const;

	//Gets the dimensions of the world map
	template<typename vectorType = float>
	sf::Vector2<vectorType> GetDimensions()
	{
		return sf::Vector2<vectorType>(static_cast<vectorType>(GetWidth()), static_cast<vectorType>(GetHeight()));
	}

	//Gets the spawnpoint of the player in the world map
	sf::Vector2<int> GetSpawnPoint() const;
	//Gets all the enemy spawnpoints in the map
	const std::vector<sf::Vector2f>& GetEnemySpawnPoints() const;
	//Gets all the enemy spawnpoints in the map
	std::vector<sf::Vector2f>& GetEnemySpawnPoints();
	//Gets the location of the exit door
	sf::Vector2f GetDoorLocation() const;
	
	//Draws the world map to the screen
	virtual void Render(sf::RenderWindow& window) override;

	//Tests to see if a tile exists at the specified location. If true, then the output reference will be modified
	bool HasTile(int x, int y, BackgroundTile& output) const;
	//Tests to see if a tile exists at the specified location.
	bool HasTile(int x, int y) const;

	//Get the difficulty level of the world map
	bool GetLevel() const;

	//Tests if the specified coordinate is within the boundaries of the map
	bool IsWithinBounds(int x, int y) const;

	//Gets a tile at the specified coordinate. If no tile exists there, null will be returned
	BackgroundTile* GetTile(int x, int y) const;
	//Gets a tile at the specified coordinate. If no tile exists there, null will be returned
	BackgroundTile* operator[](sf::Vector2<int> position) const;

	//Retrieves all the tiles that are wintin the specified rectangle
	Array2D<BackgroundTile*> GetTilesWithinRect(sf::FloatRect rect) const;

	//The destructor
	virtual ~WorldMap();
};

