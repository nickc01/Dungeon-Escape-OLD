#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include <vector>
#include "Room.h"

class WorldMap
{
	float progress;
	std::shared_ptr<Room> TopRoom;

	std::unique_ptr<std::thread> thread = nullptr;

	void Flatten();

	Array2D<std::shared_ptr<BackgroundTile>> tiles;

	sf::Vector2<int> SpawnPoint;

public:
	WorldMap();

	float GetProgress() const;

	//Generates the world map on a different thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1, the map is done generating
	void GenerateAsync(int level);
	void Generate(int level);

	int GetWidth() const;
	int GetHeight() const;

	sf::Vector2<int> GetSpawnPoint() const;

	void Render(sf::RenderWindow& window) const;

	bool HasTile(int x, int y, BackgroundTile& output) const;
	bool HasTile(int x, int y) const;

	bool IsWithinBounds(int x, int y) const;

	BackgroundTile* GetTile(int x, int y) const;
	BackgroundTile* operator[](sf::Vector2<int> position) const;
};

