#include "Room.h"
#include "Branch.h"
#include <exception>

using namespace sf;
using namespace std;

Room& Room::FindAvailableRoom()
{
	vector<Room*> AvailableRooms{};
	AllAvailableRooms(AvailableRooms);

	vector<Room*> ValidRooms{};

	for (Room* room : AvailableRooms)
	{
		for (auto& branch : room->GetBranches())
		{
			if (branch != nullptr && (*branch) == nullptr)
			{
				ValidRooms.push_back(room);
				break;
			}
		}
	}

	int randomIndex = RandomNumber(0, ValidRooms.size());
	return *ValidRooms[randomIndex];
}

void Room::AllAvailableRooms(vector<Room*>& rooms)
{
	if (find(begin(rooms),end(rooms),this) >= end(rooms))
	{
		rooms.push_back(this);
		for (auto& branchPtr : GetBranches())
		{
			auto branch = *branchPtr;
			if (branch != nullptr && branch->GetDestinationRoom() != nullptr)
			{
				branch->GetDestinationRoom()->AllAvailableRooms(rooms);
			}
		}
	}
}

void Room::AllAvailableBranches(std::vector<Branch*>& branches)
{
	for (auto branchPtr : GetBranches())
	{
		auto branch = *branchPtr;
		if (branch != nullptr)
		{
			if (find(begin(branches), end(branches), branch.get()) >= end(branches))
			{
				branches.push_back(branch.get());
				auto destinationRoom = branch->GetDestinationRoom();

				if (destinationRoom != nullptr)
				{
					destinationRoom->AllAvailableBranches(branches);
				}
			}
		}
	}
}

Room::Room(Vector2<int> center, Vector2<int> dimensions) :
	Center(center), 
	Dimensions(dimensions),
	backgroundTiles(GetWidth(),GetHeight(),nullptr)
{
	int width = GetWidth();
	int height = GetHeight();;

	for (int x = 1; x < width - 1; x++)
	{
		for (int y = 1; y < height - 1; y++)
		{
			backgroundTiles[{x, y}] = BackgroundTile::Create(Common::Sprites::blankTile);
		}
	}
	for (int y = 1; y < height - 1; y++)
	{
		backgroundTiles[{width - 1, y}] = BackgroundTile::Create(Common::Sprites::rightPiece, true);
		backgroundTiles[{0, y}] = BackgroundTile::Create(Common::Sprites::leftPiece, true);
	}
	for (int x = 1; x < width - 1; x++)
	{
		backgroundTiles[{x, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomPiece, true);
		backgroundTiles[{x, 0}] = BackgroundTile::Create(Common::Sprites::topPiece, true);
	}
	backgroundTiles[{0, 0}] = BackgroundTile::Create(Common::Sprites::topLeftPiece, true);
	backgroundTiles[{width - 1, 0}] = BackgroundTile::Create(Common::Sprites::topRightPiece, true);
	backgroundTiles[{0, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomLeftPiece, true);
	backgroundTiles[{width - 1, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomRightPiece, true);
}

Rect<int> Room::GetRect() const
{
	int left = Center.x - Dimensions.x / 2;
	int top = Center.y + Dimensions.y / 2;

	return Rect<int>(left, top, Dimensions.x, Dimensions.y);
}

Vector2<int> Room::GetCenter() const
{
	return Center;
}

void Room::SetCenter(Vector2<int> center)
{
	Center = center;
}

Vector2<int> Room::GetDimensions() const
{
	return Dimensions;
}

int Room::GetWidth() const
{
	return Dimensions.x;
}

int Room::GetHeight() const
{
	return Dimensions.y;
}


std::shared_ptr<Branch> Room::GetBranch(Direction direction)
{
	switch (direction)
	{
	case Direction::Up:
		return UpBranch;
	case Direction::Right:
		return RightBranch;
	case Direction::Down:
		return DownBranch;
	case Direction::Left:
		return LeftBranch;
	}
	return nullptr;
}

array<shared_ptr<Branch>*, 4> Room::GetBranches()
{
	auto list = std::array<std::shared_ptr<Branch>*, 4>();

	list[0] = &UpBranch;
	list[1] = &DownBranch;
	list[2] = &LeftBranch;
	list[3] = &RightBranch;

	return list;
}

std::vector<std::shared_ptr<Branch>*> Room::GetEmptyBranches()
{
	auto list = std::vector<std::shared_ptr<Branch>*>();

	if (UpBranch == nullptr)
	{
		list.push_back(&UpBranch);
	}
	if (DownBranch == nullptr)
	{
		list.push_back(&DownBranch);
	}
	if (LeftBranch == nullptr)
	{
		list.push_back(&LeftBranch);
	}
	if (RightBranch == nullptr)
	{
		list.push_back(&RightBranch);
	}

	return list;
}

void Room::SetBranch(Direction direction, std::shared_ptr<Branch> branch)
{
	switch (direction)
	{
	case Direction::Up:
		UpBranch = branch;
		break;
	case Direction::Right:
		RightBranch = branch;
		break;
	case Direction::Down:
		DownBranch = branch;
		break;
	case Direction::Left:
		LeftBranch = branch;
		break;
	default:
		break;
	}
}

bool Room::Intersects(const Room& B) const
{
	return GetRect().intersects(B.GetRect());
}

const shared_ptr<BackgroundTile>& Room::GetTile(Vector2<int> position) const
{
	return backgroundTiles[position];
}

shared_ptr<BackgroundTile>& Room::GetTile(Vector2<int> position)
{
	return backgroundTiles[position];
}

const std::shared_ptr<BackgroundTile>& Room::operator[](Vector2<int> position) const
{
	return GetTile(position);
}

std::shared_ptr<BackgroundTile>& Room::operator[](Vector2<int> position)
{
	return GetTile(position);
}


bool Room::CheckForCollisions()
{
	vector<Room*> Rooms{};

	AllAvailableRooms(Rooms);

	for (auto roomA : Rooms)
	{
		for (auto roomB : Rooms)
		{
			if (roomB == roomA)
			{
				continue;
			}
			else if (roomB->Intersects(*roomA))
			{
				return true;
			}
		}
	}
	return false;
}

void Room::AddRoomToHierarchy(shared_ptr<Room> destinationRoom)
{
	while (true)
	{

		Room& sourceRoom = FindAvailableRoom();

		auto branches = sourceRoom.GetEmptyBranches();

		if (branches.size() == 0)
		{
			continue;
		}

		int randomBranchIndex = RandomNumber(0, branches.size());

		auto branchPtr = branches[randomBranchIndex];

		auto& branch = *branchPtr;



		auto rect = sourceRoom.GetRect();


		if (branchPtr == &sourceRoom.UpBranch)
		{
			branch = make_shared<Branch>(Direction::Up);
			branch->SetStartPoint({RandomNumber(rect.left + 2,rect.left + rect.width - 2),rect.top});
		}
		else if (branchPtr == &sourceRoom.DownBranch)
		{
			branch = make_shared<Branch>(Direction::Down);
			branch->SetStartPoint({ RandomNumber(rect.left + 2,rect.left + rect.height - 2),rect.top - rect.height });
		}
		else if (branchPtr == &sourceRoom.LeftBranch)
		{
			branch = make_shared<Branch>(Direction::Left);
			branch->SetStartPoint({ rect.left,RandomNumber(rect.top - rect.height + 2,rect.top - 2) });
		}
		else if (branchPtr == &sourceRoom.RightBranch)
		{
			branch = make_shared<Branch>(Direction::Right);
			branch->SetStartPoint({ rect.left + rect.width,RandomNumber(rect.top - rect.height + 2,rect.top - 2) });
		}

		branch->SetDestinationRoom(destinationRoom);

		Vector2<int> EndPoint = branch->GetDestinationPoint();
		Vector2<int> DestinationCenter = Vector2<int>(0, 0);

		auto destRect = destinationRoom->GetRect();

		switch (branch->GetDirection())
		{
		case Direction::Up:
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y + (destRect.height / 2));
			break;
		case Direction::Right:
			DestinationCenter = Vector2<int>(EndPoint.x + (destRect.width / 2), EndPoint.y);
			break;
		case Direction::Down:
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y - (destRect.height / 2));
			break;
		case Direction::Left:
			DestinationCenter = Vector2<int>(EndPoint.x - (destRect.width / 2), EndPoint.y);
			break;
		}


		destinationRoom->SetCenter(DestinationCenter);

		auto allRooms = GetAllConnectedRooms();
		auto allBranches = GetAllConnectedBranches();

		if (CheckForCollisions())
		{
			(*branchPtr) = nullptr;
			continue;
		}
		else
		{
			return;
		}
	}
}

void Room::AddRoomToHierarchy()
{
	AddRoomToHierarchy(make_shared<Room>(Vector2<int>(0,0),Vector2<int>(RandomNumber(10,24), RandomNumber(10, 24))));
}

const std::vector<Room*> Room::GetAllConnectedRooms()
{
	std::vector<Room*> rooms{};
	AllAvailableRooms(rooms);

	return rooms;
}

const std::vector<Branch*> Room::GetAllConnectedBranches()
{
	std::vector<Branch*> branches{};
	AllAvailableBranches(branches);

	return branches;
}
