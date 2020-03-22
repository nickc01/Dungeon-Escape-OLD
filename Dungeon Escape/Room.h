#pragma once
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include "Array2D.h" //Contains the Array2D class, which is for storing objects in a 2D array
#include "BackgroundTile.h" //Contains the BackgroundTile class
#include <array> //Contains std::array, which is a fixed array type

class Branch; //Forward declare the branch class to prevent potential circular dependencies
enum class Direction; //Forward declare the direction enum to prevent potential circular dependencies

//Represents a room the player can go into
class Room
{
	sf::Vector2<int> Center; //The center of the room
	sf::Vector2<int> Dimensions; //The size of the room

	Array2D<std::shared_ptr<BackgroundTile>> backgroundTiles; //The 2D array of background tiles that make up the room

	std::shared_ptr<Branch> UpBranch = nullptr; //A branch path that branches off the top part of the room
	std::shared_ptr<Branch> DownBranch = nullptr; //A branch path that branches off the bottom part of the room
	std::shared_ptr<Branch> LeftBranch = nullptr; //A branch path that branches off the left part of the room
	std::shared_ptr<Branch> RightBranch = nullptr; //A branch path that branches off the right part of the room

	//Finds an room that has a spot avaiable for a new branch to be installed
	Room& FindAvailableRoom();
	//Gets all the rooms that are in the room hierarchy
	void FindAllRoomsInHierarchy(std::vector<Room*>& rooms);
	//Gets all the branches that are in the room hiearchy
	void FindAllBranchesInHierarchy(std::vector<Branch*>& branches);


	std::vector<sf::Vector2i> enemySpawnPoints; //The list of all enemy spawnpoints that are in the room

	//Adds a new enemy spawnpoint to the room
	void AddEnemySpawnpoint();

public:

	static constexpr int MaxRoomWidth = 12; //The maximum width of a room
	static constexpr int MaxRoomHeight = 12; //The maximum height of a room

	static constexpr int MinRoomWidth = 7; //The minimum width of a room
	static constexpr int MinRoomHeight = 7; //The minimum height of a room

	//Contructs a new room
	Room(sf::Vector2<int> center, sf::Vector2<int> dimensions, int enemySpawnPoints = 0);

	//Retrieves the rect bounds of the room
	sf::Rect<int> GetRect() const;
	//Gets the center coordinates of the room
	sf::Vector2<int> GetCenter() const;
	//Sets the center of the room
	void SetCenter(sf::Vector2<int> center);

	//Gets the dimensions of the room
	sf::Vector2<int> GetDimensions() const;

	//Gets the width of the room
	int GetWidth() const;
	//Gets the height of the room
	int GetHeight() const;

	//Gets the branch that is installed on the specified side of the room
	std::shared_ptr<Branch> GetBranch(Direction side);
	//Gets all 4 possible branch sides of the room
	std::array<std::shared_ptr<Branch>*,4> GetBranches();
	//Gets only the branches that are empty
	std::vector<std::shared_ptr<Branch>*> GetEmptyBranches();
	//Adds the branch to a specific side of the room
	void SetBranch(Direction direction, std::shared_ptr<Branch> branch);

	//Returns the list of all enemy spawnpoints
	const std::vector<sf::Vector2i>& GetEnemySpawnPoints() const;
	//Returns the list of all enemy spawnpoints
	std::vector<sf::Vector2i>& GetEnemySpawnPoints();

	//Checks if the room intersects with a specified background tile
	bool Intersects(const BackgroundTile& tile) const;
	//Checks to see if this room intersects with another room
	bool Intersects(const Room& B) const;

	//Gets a room's background tile at the specified position
	const std::shared_ptr<BackgroundTile>& GetTile(sf::Vector2<int> position) const;
	//Gets a room's background tile at the specified position
	std::shared_ptr<BackgroundTile>& GetTile(sf::Vector2<int> position);

	//Gets a room's background tile at the specified position
	const std::shared_ptr<BackgroundTile>& operator[](sf::Vector2<int> position) const;
	//Gets a room's background tile at the specified position
	std::shared_ptr<BackgroundTile>& operator[](sf::Vector2<int> position);


	//Checks to see if a newly added room will collide with anything in the room hierarchy
	bool CheckForCollision(const Room* addedRoom);

	//Adds a new child room to the room hierarchy
	void AddRoomToHierarchy(std::shared_ptr<Room> room);
	//Constructs a new room to be added to the room hierarchy
	void AddRoomToHierarchy(int enemySpawnPoints = 0);

	//Gets all the room in the room hierarchy
	const std::vector<Room*> GetAllRoomsInHierarchy();

	//Gets all the branches in the room hierarchy
	const std::vector<Branch*> GetAllBranchesInHierarchy();
};

