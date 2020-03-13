#include "Room.h"
#include "Renderer.h"
#include "Branch.h"
#include <exception>

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

Room::Room(Vector2I center, Vector2I dimensions) :
	Center(center), 
	Dimensions(dimensions),
	backgroundTiles(GetWidth(),GetHeight(),nullptr)
{
	int width = GetWidth();
	int height = GetHeight();

	//Vector2I bottomLeft = GetRect().BottomLeft();

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
		//backgroundTiles[{width - 1, y}] = make_shared<BackgroundTile>(Vector2I(width - 1, y),Common::Sprites::rightPiece,true);
		//backgroundTiles[{0, y}] = make_shared<BackgroundTile>(Vector2I(0, y), Common::Sprites::leftPiece,true);
	}
	for (int x = 1; x < width - 1; x++)
	{
		backgroundTiles[{x, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomPiece, true);
		backgroundTiles[{x, 0}] = BackgroundTile::Create(Common::Sprites::topPiece, true);
		//backgroundTiles[{x, height - 1}] = make_shared<BackgroundTile>(Vector2I(x, height - 1), Common::Sprites::bottomPiece, true);
		//backgroundTiles[{x, 0}] = make_shared<BackgroundTile>(Vector2I(x, 0), Common::Sprites::topPiece,true);
	}
	backgroundTiles[{0, 0}] = BackgroundTile::Create(Common::Sprites::topLeftPiece, true);
	backgroundTiles[{width - 1, 0}] = BackgroundTile::Create(Common::Sprites::topRightPiece, true);
	backgroundTiles[{0, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomLeftPiece, true);
	backgroundTiles[{width - 1, height - 1}] = BackgroundTile::Create(Common::Sprites::bottomRightPiece, true);
	//backgroundTiles[{0, 0}] = make_shared<BackgroundTile>(Vector2I(0, 0), Common::Sprites::topLeftPiece,true);
	//backgroundTiles[{width - 1, 0}] = make_shared<BackgroundTile>(Vector2I(width - 1, 0), Common::Sprites::topRightPiece,true);
	//backgroundTiles[{0, height - 1}] = make_shared<BackgroundTile>(Vector2I(0, height - 1), Common::Sprites::bottomLeftPiece,true);
	//backgroundTiles[{width - 1, height - 1}] = make_shared<BackgroundTile>(Vector2I(width - 1, height - 1), Common::Sprites::bottomRightPiece,true);
}

RectI Room::GetRect() const
{
	return RectI(Center, Dimensions);
}

Vector2I Room::GetCenter() const
{
	return Center;
}

void Room::SetCenter(Vector2I center)
{
	Center = center;
}

Vector2I Room::GetDimensions() const
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

int Room::BottomHeight() const
{
	return GetRect().BottomHeight();
}

int Room::TopHeight() const
{
	return GetRect().TopHeight();
}

int Room::LeftWidth() const
{
	return GetRect().LeftWidth();
}

int Room::RightWidth() const
{
	return GetRect().RightWidth();
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
	return GetRect().Intersects(B.GetRect());
}

const shared_ptr<BackgroundTile>& Room::GetTile(Vector2I position) const
{
	return backgroundTiles[position];
}

shared_ptr<BackgroundTile>& Room::GetTile(Vector2I position)
{
	return backgroundTiles[position];
}

const std::shared_ptr<BackgroundTile>& Room::GetTileAbsolutePosition(Vector2I position) const
{
	return GetTile(position - GetRect().BottomLeft());
}

std::shared_ptr<BackgroundTile>& Room::GetTileAbsolutePosition(Vector2I position)
{
	return GetTile(position - GetRect().BottomLeft());
}

const std::shared_ptr<BackgroundTile>& Room::operator[](Vector2I position) const
{
	return GetTile(position);
}

std::shared_ptr<BackgroundTile>& Room::operator[](Vector2I position)
{
	return GetTile(position);
}

/*void Room::Render(sf::RenderWindow& window)
{
	for (Room* room : GetAllConnectedRooms())
	{
		auto bottomLeft = room->GetRect().BottomLeft();

		for (int x = 0; x < room->GetWidth(); x++)
		{
			for (int y = 0; y < room->GetHeight(); y++)
			{
				auto tile = room->backgroundTiles[{x, y}];
				if (tile != nullptr)
				{
					window.draw(tile->GetSprite());
				}
			}
		}
	}

	for (Branch* branch : GetAllConnectedBranches())
	{
		for (auto& tile : branch->GetTiles())
		{
			window.draw(tile->GetSprite());
		}
	}
}*/

/*void Room::Render() const
{
	auto oldCameraPos = Renderer::CameraPosition;

	Renderer::CameraPosition += (Dimensions / 2);

	Renderer::CameraPosition -= Center;

	for (int x = 0; x < GetWidth(); x++)
	{
		for (int y = 0; y < GetHeight(); y++)
		{
			auto ptr = GetTile({ x,y });
			if (ptr != nullptr)
			{
				ptr->Render();
			}
		}
	}

	Renderer::CameraPosition = oldCameraPos;
}*/

/*void Room::GetAllConnections(std::vector<const Room*>& OutRooms, std::vector<const Branch*>& OutBranches) const
{
	OutRooms.push_back(this);

	for (auto& branch : GetBranches())
	{
		if (branch != nullptr && find(begin(OutBranches), end(OutBranches), branch) < end(OutBranches))
		{
			OutBranches.push_back(branch.get());
			auto destinationRoom = branch->GetDestinationRoom();
			if (destinationRoom != nullptr && find(begin(OutRooms),end(OutRooms),destinationRoom) < end(OutRooms))
			{
				branch->GetDestinationRoom()->GetAllConnections(OutRooms, OutBranches);
			}
		}
	}
}*/

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

		//auto dimensions = destinationRoom->GetDimensions();

		//auto rect = destinationRoom->GetRect();
		auto rect = sourceRoom.GetRect();


		if (branchPtr == &sourceRoom.UpBranch)
		{
			branch = make_shared<Branch>(Direction::Up);
			branch->SetStartPoint({RandomNumber(rect.Left + 2,rect.Right - 2),rect.Top});
		}
		else if (branchPtr == &sourceRoom.DownBranch)
		{
			branch = make_shared<Branch>(Direction::Down);
			branch->SetStartPoint({ RandomNumber(rect.Left + 2,rect.Right - 2),rect.Bottom });
		}
		else if (branchPtr == &sourceRoom.LeftBranch)
		{
			branch = make_shared<Branch>(Direction::Left);
			branch->SetStartPoint({ rect.Left,RandomNumber(rect.Bottom + 2,rect.Top - 2) });
		}
		else if (branchPtr == &sourceRoom.RightBranch)
		{
			branch = make_shared<Branch>(Direction::Right);
			branch->SetStartPoint({ rect.Right,RandomNumber(rect.Bottom + 2,rect.Top - 2) });
		}

		branch->SetDestinationRoom(destinationRoom);

		Vector2I EndPoint = branch->GetDestinationPoint();
		//RectI destinationRect = destinationRoom->GetRect();
		Vector2I DestinationCenter = Vector2I(0, 0);

		switch (branch->GetDirection())
		{
		case Direction::Up:
			DestinationCenter = Vector2I(EndPoint.x, EndPoint.y + (destinationRoom->BottomHeight()));
			break;
		case Direction::Right:
			DestinationCenter = Vector2I(EndPoint.x + (destinationRoom->LeftWidth()), EndPoint.y);
			break;
		case Direction::Down:
			DestinationCenter = Vector2I(EndPoint.x, EndPoint.y - (destinationRoom->TopHeight()));
			break;
		case Direction::Left:
			DestinationCenter = Vector2I(EndPoint.x - (destinationRoom->RightWidth()), EndPoint.y);
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
	AddRoomToHierarchy(make_shared<Room>(Vector2I(0,0),Vector2I(RandomNumber(10,24), RandomNumber(10, 24))));
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
