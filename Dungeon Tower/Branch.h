#pragma once
#include "Direction.h"
#include "Common.h"
#include "Vector2.h"
#include <vector>
#include "BackgroundTile.h"

class Room;

class Branch
{
	//std::shared_ptr<Room> sourceRoom = nullptr;
	std::shared_ptr<Room> destinationRoom = nullptr;

	int firstLength = 0;
	int secondLength = 0;
	int thirdLength = 0;
	Direction midDirection = Direction::Up;
	Direction direction;

	Vector2 startPoint = Vector2(0, 0);

	std::vector<std::shared_ptr<BackgroundTile>> tiles;

	void CreateTiles();

public:

	Branch(Direction direction);

	//Branch(const Branch& branch) = delete;
	//Branch(Branch&& branch);

	//Branch& operator=(const Branch& branch) = delete;
	//Branch& operator=(Branch&& branch);


	//std::shared_ptr<Room> GetSourceRoom();
	std::shared_ptr<Room> GetDestinationRoom();

	//void SetSourceRoom(std::shared_ptr<Room> room);
	void SetDestinationRoom(std::shared_ptr<Room> room);

	Direction GetDirection();

	void SetStartPoint(Vector2 point);
	Vector2 GetStartPoint();
	Vector2 GetDestinationPoint();

	const std::vector<std::shared_ptr<BackgroundTile>>& GetTiles();


	int GetStartLength() const;
	Direction GetStartDirection() const;

	int GetMidLength() const;
	Direction GetMidDirection() const;

	int GetEndLength() const;
	Direction GetEndDirection() const;

	//Vector2 GetStartPoint();
};

