#pragma once
#include <thread>
#include <memory>
#include <vector>
#include "Displayable.h"
#include "Room.h"

class WorldMap
{
	std::vector<std::unique_ptr<Displayable>> Tiles;

	std::shared_ptr<Room> TopRoom;
	//void GenerateRoom(Vector2 center, Vector2 dimensions);


	float progress;
	std::unique_ptr<std::thread> thread = nullptr;

	void Flatten();

	Array2D<std::shared_ptr<BackgroundTile>> backgroundLayer;
	//Array2D<std::shared_ptr<BackgroundTile>> spriteLayer;


	Vector2 BottomLeft;
	Vector2 TopRight;

	Vector2 SpawnPoint;

public:
	WorldMap();

	float GetProgress() const;

	//Generates the world map on a different thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1, the map is done generating
	void GenerateAsync(int level);
	void Generate(int level);

	int GetWidth() const;
	int GetHeight() const;

	Vector2 GetSpawnPoint() const;



	Array2D<std::shared_ptr<BackgroundTile>>& GetBackgroundLayer();
};

