#pragma once
#include <thread>
#include <memory>
#include <vector>
#include "Displayable.h"
#include "Room.h"

class WorldMap
{
	std::vector<std::unique_ptr<Displayable>> Tiles;

	Vector2 SpawnPoint;
	std::shared_ptr<Room> TopRoom;
	//void GenerateRoom(Vector2 center, Vector2 dimensions);


	float progress;
	std::unique_ptr<std::thread> thread = nullptr;
public:
	WorldMap();

	float GetProgress();

	//Generates the world map on a different thread. you can get the generation progress via GetProgress(). When GetProgress() equals 1, the map is done generating
	void GenerateAsync(int level);
	void Generate(int level);

	std::shared_ptr<Room> GetFirstRoom();
};

