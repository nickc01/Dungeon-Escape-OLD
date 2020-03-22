#pragma once
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering

#include "Direction.h" //Contains the Direction Enum for specifying the direction
#include "Common.h" //Contains many common game functions and variables
#include <vector> //Contains std::vector for storing objects in an array
#include "BackgroundTile.h" //Contains the BackgroundTile class

class Room; //Forward declare the room class to prevent circular dependencies

//Represents a branching path to a new room
class Branch
{
	bool startPointUpdated = true; //Whether the start point has been updated or not

	std::shared_ptr<Room> destinationRoom = nullptr; //The pointer to the destination room

	//The room is split into three partitions, each with their own lengths
	int startLength = 0; //The length of the first section of path
	int midLength = 0; //The length of the second section of path
	int endLength = 0;//The length of the third section of path
	Direction midSectionDirection = Direction::Up; //The direction the middle section will be traveling in
	Direction direction; //The main direction the branch path is taking

	sf::Vector2<int> startPoint = sf::Vector2<int>(0, 0); //The point where the branch path starts

	std::vector<std::shared_ptr<BackgroundTile>> tiles; //The tiles that make up the branch path
	std::vector<bool> canOverlap; //An array that determines which tiles can safely overlap rooms or not

	//Creates all the tiles that make up the branch
	void CreateTiles();

	//Builds a single tile
	void BuildTile(const sf::Sprite& sprite, sf::Vector2<int> position, bool canOverlap = false);
	//Builds a new joint piece at the specified position
	void BuildJointPiece(Direction direction, sf::Vector2i position);
	//Builds a new straight piece at the specified position
	void BuildStraightPiece(Direction direction, sf::Vector2i position);
	//Builds a curve piece at the specified position
	void BuildCurvePiece(Direction from, Direction to, sf::Vector2i position);

public:

	//Constructs a new branch pointing in the specified direction
	Branch(Direction direction);

	//Gets the pointer to the destination room
	std::shared_ptr<Room> GetDestinationRoom();

	//Sets the pointer to the destination room
	void SetDestinationRoom(std::shared_ptr<Room> room);

	//Gets the main direction the branch is traveling in
	Direction GetDirection();

	//Sets the starting point for the branch
	void SetStartPoint(sf::Vector2<int> point);
	//Gets the starting point of the branch
	sf::Vector2<int> GetStartPoint();
	//Gets the destination point of the branch
	sf::Vector2<int> GetDestinationPoint();

	//Gets all the tiles that make up the branch
	const std::vector<std::shared_ptr<BackgroundTile>>& GetTiles();

	//Checks to see if a branch overlaps another one
	bool Intersects(Branch& other);

	//Checks to see if the branch is colliding with any of the rooms or branches in the room hierarchy
	bool CheckForCollisions(Room* TopRoom);

	//Gets the length of the first section of the branch path
	int GetStartLength() const;
	//Gets the direction of the first section
	Direction GetStartDirection() const;

	//Gets the length of the middle section of the branch path
	int GetMidLength() const;
	//Gets the direction of the middle section
	Direction GetMidDirection() const;

	//Gets the length of the last section of the branch path
	int GetEndLength() const;
	//Gets the direction of the last section
	Direction GetEndDirection() const;
};

