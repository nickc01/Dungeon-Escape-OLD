#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include <vector>
#include "Room.h"
#include "Renderable.h"
#include <atomic>

class WorldMap : public Renderable
{
	std::atomic<float> progress;
	std::shared_ptr<Room> TopRoom;

	std::unique_ptr<std::thread> thread = nullptr;

	void Flatten();

	std::vector<sf::Vector2f> enemySpawnPoints{};

	sf::Vector2f doorLocation;

	Array2D<std::shared_ptr<BackgroundTile>> tiles;

	sf::Vector2u tileSize;

	sf::Vector2<int> SpawnPoint;

	int level = 0;

public:
	WorldMap();

	const std::atomic<float>& GetProgress() const;

	//Generates the world map on a different thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1, the map is done generating
	void GenerateAsync(int level);
	void Generate(int level);

	int GetWidth() const;
	int GetHeight() const;

	sf::Vector2u GetTileSize() const;

	template<typename vectorType = float>
	sf::Vector2<vectorType> GetDimensions()
	{
		return sf::Vector2<vectorType>(static_cast<vectorType>(GetWidth()), static_cast<vectorType>(GetHeight()));
	}

	sf::Vector2<int> GetSpawnPoint() const;
	const std::vector<sf::Vector2f>& GetEnemySpawnPoints() const;
	std::vector<sf::Vector2f>& GetEnemySpawnPoints();

	sf::Vector2f GetDoorLocation() const;
	

	virtual void Render(sf::RenderWindow& window) override;

	bool HasTile(int x, int y, BackgroundTile& output) const;
	bool HasTile(int x, int y) const;

	bool GetLevel() const;

	bool IsWithinBounds(int x, int y) const;

	BackgroundTile* GetTile(int x, int y) const;
	BackgroundTile* operator[](sf::Vector2<int> position) const;

	Array2D<BackgroundTile*> GetTilesWithinRect(sf::FloatRect rect) const;


	virtual ~WorldMap();
};

