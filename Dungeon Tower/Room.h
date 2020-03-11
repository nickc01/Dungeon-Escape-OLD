#pragma once
#include "Vector2.h"
#include "Rect.h"
#include "Array2D.h"
#include "BackgroundTile.h"
#include <array>

class Branch;
enum class Direction;

class Room
{
	Vector2 Center;
	Vector2 Dimensions;

	Array2D<std::shared_ptr<BackgroundTile>> backgroundTiles;

	std::shared_ptr<Branch> UpBranch = nullptr;
	std::shared_ptr<Branch> DownBranch = nullptr;
	std::shared_ptr<Branch> LeftBranch = nullptr;
	std::shared_ptr<Branch> RightBranch = nullptr;

	Room& FindAvailableRoom();
	void AllAvailableRooms(std::vector<Room*>& rooms);
	void AllAvailableBranches(std::vector<Branch*>& branches);


	//void GetSelfConnections(std::vector<Room*>& OutRooms, std::vector<Branch*>& OutBranches, Direction excludedDirection);


public:
	Room(Vector2 center, Vector2 dimensions);

	Rect GetRect() const;
	Vector2 GetCenter() const;
	void SetCenter(Vector2 center);

	Vector2 GetDimensions() const;

	int GetWidth() const;
	int GetHeight() const;

	std::shared_ptr<Branch> GetBranch(Direction direction);
	std::array<std::shared_ptr<Branch>*,4> GetBranches();
	std::vector<std::shared_ptr<Branch>*> GetEmptyBranches();
	void SetBranch(Direction direction, std::shared_ptr<Branch> branch);


	bool Intersects(const Room& B) const;

	const std::shared_ptr<BackgroundTile>& GetTile(Vector2 position) const;
	std::shared_ptr<BackgroundTile>& GetTile(Vector2 position);

	const std::shared_ptr<BackgroundTile>& GetTileAbsolutePosition(Vector2 position) const;
	std::shared_ptr<BackgroundTile>& GetTileAbsolutePosition(Vector2 position);

	const std::shared_ptr<BackgroundTile>& operator[](Vector2 position) const;
	std::shared_ptr<BackgroundTile>& operator[](Vector2 position);


	void Render() const;

	//void GetAllConnections(std::vector<const Room*>& OutRooms, std::vector<const Branch*>& OutBranches) const;

	bool CheckForCollisions();

	void AddRoomToHierarchy(std::shared_ptr<Room> room);
	void AddRoomToHierarchy();

	const std::vector<Room*> GetAllConnectedRooms();
	const std::vector<Branch*> GetAllConnectedBranches();
	//~Room();
};

