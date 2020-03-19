#include "Enemy.h"
#include "Player.h"
#include <algorithm>
#include "Math.h"
#include <list>
#include <functional>
#include "ThreadPool.h"

using namespace std;
using namespace sf;

namespace
{
	struct Node
	{
		Vector2i MapPosition;
		shared_ptr<Node> PreviousNode;
		int g;
		int n;
		int f;

		Node(Vector2i mapPosition, shared_ptr<Node> previousNode, int g, int n) :
			MapPosition(mapPosition),
			PreviousNode(previousNode),
			g(g),
			n(n),
			f(g + n)
		{

		}
	};

	std::array<std::tuple<BackgroundTile*, Vector2i>,4> GetNeighboringTiles(const WorldMap& map, Vector2i position)
	{
		std::array <Vector2i, 4> positions;
		std::array<std::tuple<BackgroundTile*,Vector2i>, 4> Tiles;

		positions[0] = Vector2i(position.x + 1, position.y);
		positions[1] = Vector2i(position.x - 1, position.y);
		positions[2] = Vector2i(position.x, position.y + 1);
		positions[3] = Vector2i(position.x, position.y - 1);

		for (int i = 0; i < positions.size(); i++)
		{
			auto& pos = positions[i];
			Tiles[i] = {map.GetTile(pos.x,pos.y),pos};
		}
		return Tiles;
	}
}

PathResult Enemy::GeneratePathToPlayer()
{
	const Player* player = Player::GetCurrentPlayer();
	if (player == nullptr)
	{
		return PathResult();
	}

	auto playerPos = player->GetSprite()->getPosition();
	auto enemyPos = GetSprite()->getPosition();

	auto mapTileSize = map.GetTileSize();

	Vector2i target = Vector2i(playerPos.x / mapTileSize.x,playerPos.y / mapTileSize.y);
	Vector2i source = Vector2i(enemyPos.x / mapTileSize.x, enemyPos.y / mapTileSize.y);

	std::list<shared_ptr<Node>> OpenList;
	OpenList.push_back(make_shared<Node>(source,nullptr,0,Math::DistanceBetweenVectors(target,source)));
	//shared_ptr<Node>

	std::list<shared_ptr<Node>> ClosedList;

	shared_ptr<Node> targetNode = nullptr;

	while (OpenList.size() > 0)
	{
		OpenList.sort([](std::shared_ptr<Node>& A, std::shared_ptr<Node>& B)
			{
				return A->f > B->f;
			});
		/*sort(begin(OpenList), end(OpenList), [](std::shared_ptr<Node>& A, std::shared_ptr<Node>& B)
			{
				return A->f > B->f;
			});*/

		auto currentTile = OpenList.back();
		//auto currentTile = OpenList[OpenList.size() - 1];
		if (currentTile->MapPosition == target)
		{
			targetNode = currentTile;
			break;
		}
		OpenList.pop_back();
		ClosedList.push_back(currentTile);

		for (auto& neighbor : GetNeighboringTiles(map, currentTile->MapPosition))
		{
			BackgroundTile* tile = get<0>(neighbor);
			Vector2i tilePosition = get<1>(neighbor);

			shared_ptr<Node> newNode = make_shared<Node>(tilePosition, currentTile,currentTile->n + 1,Math::DistanceBetweenVectors(tilePosition,target));

			if (tile != nullptr && !tile->IsCollidable())
			{
				bool notInList = true;
				for (auto& node : OpenList)
				{
					if (node->MapPosition == tilePosition)
					{
						notInList = false;
						break;
					}
				}
				if (notInList)
				{
					for (auto& node : ClosedList)
					{
						if (node->MapPosition == tilePosition)
						{
							notInList = false;
							break;
						}
					}
				}

				if (notInList)
				{
					OpenList.push_back(newNode);
				}
			}
		}
	}
	if (targetNode == nullptr)
	{
		return PathResult();
	}

	shared_ptr<Node> nodePath = targetNode;

	std::list<sf::Vector2f> Path;

	while (nodePath != nullptr)
	{
		auto position = nodePath->MapPosition;
		Path.emplace_front(static_cast<float>(position.x * mapTileSize.x),static_cast<float>(position.y * mapTileSize.y));
		nodePath = nodePath->PreviousNode;
	}

	//reverse(begin(Path), end(Path));

	return PathResult(std::move(Path), true);
}
std::shared_ptr<PathAsyncResult> Enemy::GeneratePathToPlayerAsync()
{
	std::shared_ptr<PathAsyncResult> result = make_shared<PathAsyncResult>();

	ThreadPool::AddToQueue([result,enemy = this]() 
		{
			auto finalResult = enemy->GeneratePathToPlayer();
			result->result = std::move(finalResult);
			result->done = true;
		});

	return result;
	/*return std::async(std::launch::async, [&]() 
		{
			return GeneratePathToPlayer();
		});*/
}

int Enemy::GetStrength() const
{
	return Strength;
}

Enemy::Enemy(const WorldMap& map, int strength, bool enableRendering, bool enableCollision) :
	AnimatedEntity(map,true,1.0f / 12.0f, enableRendering, enableCollision),
	map(map),
	Strength(strength)
{

}

/*void Enemy::Update(sf::Time dt)
{

}*/

PathResult::PathResult()
{

}

PathResult::PathResult(const std::list<sf::Vector2f>& path, bool success) :
	Path(path),
	Success(success)
{

}

PathResult::PathResult(std::list<sf::Vector2f>&& path, bool success) :
	Path(std::move(path)),
	Success(success)
{
}
