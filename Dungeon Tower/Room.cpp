#include "Room.h"
#include "Branch.h"
#include <exception>

using namespace sf;
using namespace std;

namespace
{
	template<typename rectType>
	bool RectIntersects(sf::Rect<rectType> rectA, sf::Rect<rectType> rectB)
	{
		auto leftA = rectA.left;
		auto rightA = rectA.left + rectA.width;
		auto topA = rectA.top;
		auto bottomA = rectA.top - rectA.height;

		auto leftB = rectB.left;
		auto rightB = rectB.left + rectB.width;
		auto topB = rectB.top;
		auto bottomB = rectB.top - rectB.height;

		if (leftA < rightB && rightA > leftB&& topA > bottomB&& bottomA < topB)
		{
			return true;
		}
		return false;
	}
}

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
	int height = GetHeight();

	auto rect = GetRect();


	for (int x = 1; x <= width - 2; x++)
	{
		for (int y = 1; y <= height - 2; y++)
		{
			backgroundTiles[{x, y}] = BackgroundTile::Create(Common::GetCenterSprite());
		}
	}
	for (int y = 1; y <= height - 2; y++)
	{
		backgroundTiles[{width - 1, y}] = BackgroundTile::Create(Common::Sprites::rightPiece);
		backgroundTiles[{0, y}] = BackgroundTile::Create(Common::Sprites::leftPiece);
	}
	for (int x = 1; x <= width - 2; x++)
	{
		backgroundTiles[{x, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomPiece);
		backgroundTiles[{x, 0}] = BackgroundTile::Create(Common::Sprites::topPiece);
	}
	backgroundTiles[{0, 0}] = BackgroundTile::Create(Common::Sprites::topLeftPiece);
	backgroundTiles[{width - 1, 0}] = BackgroundTile::Create(Common::Sprites::topRightPiece);
	backgroundTiles[{0, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomLeftPiece);
	backgroundTiles[{width - 1, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomRightPiece);
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

bool Room::Intersects(const BackgroundTile& tile) const
{
	auto rectA = Rect<float>(GetRect());
	auto rectB = tile.GetSprite().getGlobalBounds();

	//rectB.left *= tile.GetSprite().getTextureRect().width;
	//rectB.top *= tile.GetSprite().getTextureRect().height;

	return RectIntersects(rectA, rectB);
}

bool Room::Intersects(const Room& B) const
{

	auto rectA = GetRect();
	auto rectB = B.GetRect();

	return RectIntersects(rectA, rectB);

	/*if (leftA > rightB || rightA < leftB || topA < bottomB || bottomA > topB)
	{
		return false;
	}
	else
	{
		return true;
	}*/



	//return GetRect().intersects(B.GetRect());
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

bool Room::CheckForCollision(const Room* addedRoom)
{
	vector<Room*> Rooms{};

	AllAvailableRooms(Rooms);

	for (auto room : Rooms)
	{
		if (room == addedRoom)
		{
			continue;
		}
		else if (room->Intersects(*addedRoom))
		{
			return true;
		}
	}

	vector<Branch*> Branches{};

	AllAvailableBranches(Branches);

	for (auto branch : Branches)
	{
		for (auto tile : branch->GetTiles())
		{
			if (addedRoom->Intersects(*tile))
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

		int left = rect.left;
		int right = rect.left + rect.width - 1;
		int top = rect.top - 1;
		int bottom = rect.top - rect.height;


		if (branchPtr == &sourceRoom.UpBranch)
		{
			branch = make_shared<Branch>(Direction::Up);
			branch->SetStartPoint({RandomNumber(left + 2,right - 2),top});
		}
		else if (branchPtr == &sourceRoom.DownBranch)
		{
			branch = make_shared<Branch>(Direction::Down);
			branch->SetStartPoint({ RandomNumber(left + 2,right - 2),bottom });
		}
		else if (branchPtr == &sourceRoom.LeftBranch)
		{
			branch = make_shared<Branch>(Direction::Left);
			branch->SetStartPoint({ left,RandomNumber(bottom + 2,top - 2) });
		}
		else if (branchPtr == &sourceRoom.RightBranch)
		{
			branch = make_shared<Branch>(Direction::Right);
			branch->SetStartPoint({ right,RandomNumber(bottom + 2,top - 2) });
		}

		branch->SetDestinationRoom(destinationRoom);

		Vector2<int> EndPoint = branch->GetDestinationPoint();
		Vector2<int> DestinationCenter = Vector2<int>(0, 0);

		auto destRect = destinationRoom->GetRect();
		auto destCenter = destinationRoom->Center;

		switch (branch->GetDirection())
		{
		case Direction::Up:
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y + (destCenter.y - (destRect.top - destRect.height)));
			//DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y + (destRect.height / 2));
			break;
		case Direction::Right:
			//DestinationCenter = Vector2<int>(EndPoint.x + (destRect.width / 2), EndPoint.y);
			//DestinationCenter = Vector2<int>(EndPoint.x - (destRect.width / 2), EndPoint.y);
			DestinationCenter = Vector2<int>(EndPoint.x + (destCenter.x - destRect.left), EndPoint.y);
			break;
		case Direction::Down:
			DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y - (destRect.top - destCenter.y) + 1);
			//DestinationCenter = Vector2<int>(EndPoint.x, EndPoint.y - (destRect.height / 2));
			break;
		case Direction::Left:
			//DestinationCenter = Vector2<int>(EndPoint.x - (destRect.width / 2), EndPoint.y);
			DestinationCenter = Vector2<int>(EndPoint.x - (destRect.left + destRect.width - destCenter.x) + 1, EndPoint.y);
			break;
		}


		destinationRoom->SetCenter(DestinationCenter);

		auto allRooms = GetAllConnectedRooms();
		auto allBranches = GetAllConnectedBranches();

		if (CheckForCollision(destinationRoom.get()) || branch->CheckForCollisions(this))
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
	AddRoomToHierarchy(make_shared<Room>(Vector2<int>(0,0),Vector2<int>(RandomNumber(MinRoomWidth,MaxRoomWidth), RandomNumber(MinRoomHeight, MaxRoomHeight))));
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
