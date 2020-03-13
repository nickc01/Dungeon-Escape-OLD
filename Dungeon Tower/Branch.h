#pragma once
#include <SFML/Graphics.hpp>

#include "Direction.h"
#include "Common.h"
#include <vector>
#include "BackgroundTile.h"

class Room;

class Branch
{
	std::shared_ptr<Room> destinationRoom = nullptr;

	int firstLength = 0;
	int secondLength = 0;
	int thirdLength = 0;
	Direction midDirection = Direction::Up;
	Direction direction;

	sf::Vector2<int> startPoint = sf::Vector2<int>(0, 0);

	std::vector<std::shared_ptr<BackgroundTile>> tiles;

	void CreateTiles();

public:

	Branch(Direction direction);

	std::shared_ptr<Room> GetDestinationRoom();

	void SetDestinationRoom(std::shared_ptr<Room> room);

	Direction GetDirection();

	void SetStartPoint(sf::Vector2<int> point);
	sf::Vector2<int> GetStartPoint();
	sf::Vector2<int> GetDestinationPoint();

	const std::vector<std::shared_ptr<BackgroundTile>>& GetTiles();


	int GetStartLength() const;
	Direction GetStartDirection() const;

	int GetMidLength() const;
	Direction GetMidDirection() const;

	int GetEndLength() const;
	Direction GetEndDirection() const;
};

