#pragma once

#include <SFML/Graphics.hpp>
#include <thread>
#include <memory>
#include <vector>
#include "Displayable.h"
#include "Room.h"

class WorldMap
{
	//std::vector<std::unique_ptr<Sprite>> Tiles;

	std::shared_ptr<Room> TopRoom;
	//void GenerateRoom(Vector2I center, Vector2I dimensions);


	float progress;
	std::unique_ptr<std::thread> thread = nullptr;

	void Flatten();

	Array2D<std::shared_ptr<BackgroundTile>> backgroundLayer;

	Vector2I SpawnPoint;

public:
	WorldMap();

	float GetProgress() const;

	//Generates the world map on a different thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1, the map is done generating
	void GenerateAsync(int level);
	void Generate(int level);

	int GetWidth() const;
	int GetHeight() const;

	Vector2I GetSpawnPoint() const;

	void Render(sf::RenderWindow& window) const;



	//Array2D<std::shared_ptr<BackgroundTile>>& GetBackgroundLayer();
};

