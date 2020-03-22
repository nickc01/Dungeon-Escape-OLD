#include "Enemy.h" //Contains the Enemy class
#include "Player.h" //Contains the Player class that represents the player
#include <algorithm> //Contains many commonly used algorithms such as std::sort and std::find
#include "Math.h" //Contains many commonly used math functions
#include <list> //Contains std::list for storing items in a linked list
#include "ThreadPool.h" //Contains the thread pool for running code asynchronously

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

namespace
{
	//Represents a place the algorithm is able to walk over
	struct Node
	{
		//The position of the node
		Vector2i MapPosition;

		//The previous node that connects to this one
		shared_ptr<Node> PreviousNode;
		int g; //G represents how many spaces the algorithm has taken to get to this spot
		int n; //N represents the approximate distance to the destination
		int f; //F is the sum of G and N

		//Constructs a new node
		Node(Vector2i mapPosition, shared_ptr<Node> previousNode, int g, int n) :
			MapPosition(mapPosition),
			PreviousNode(previousNode),
			g(g),
			n(n),
			f(g + n)
		{

		}
	};


	//Gets all potental tiles that are neighboring the one at the specified position
	std::array<std::tuple<BackgroundTile*, Vector2i>,4> GetNeighboringTiles(const WorldMap& map, Vector2i position)
	{
		std::array <Vector2i, 4> positions; //Stores the four possible neighboring positions
		std::array<std::tuple<BackgroundTile*,Vector2i>, 4> Tiles; //Stores the four neighboring tiles and their positions

		//Setup the neighboring positions
		positions[0] = Vector2i(position.x + 1, position.y);
		positions[1] = Vector2i(position.x - 1, position.y);
		positions[2] = Vector2i(position.x, position.y + 1);
		positions[3] = Vector2i(position.x, position.y - 1);

		//Loop over all the positions
		for (int i = 0; i < positions.size(); i++)
		{
			//Get the tiles at the corresponding positions
			auto& pos = positions[i];
			Tiles[i] = {map.GetTile(pos.x,pos.y),pos};
		}
		//Return the tiles and their positions
		return Tiles;
	}
}

//Uses a A* pathfinding algorithm to find a path to the player
PathResult Enemy::GeneratePathToPlayer()
{
	//Get the current player in the game
	const Player* player = Player::GetCurrentPlayer();

	/*If
		A: The enemy's set sprite is null
		B: The player is nonexistent
		C: The player is not alive
		D: The player has no sprite set
	*/
	if (GetSprite() == nullptr || player == nullptr || !player->IsAlive() || player->GetSprite() == nullptr)
	{
		//Return a blank results object
		return PathResult();
	}

	//Get the player's position
	auto playerPos = player->GetSprite()->getPosition();

	//Get the enemy position
	auto enemyPos = GetSprite()->getPosition();

	//Get the size of each tile
	auto mapTileSize = GetMap().GetTileSize();

	//Gets the player's position in map coordinates
	Vector2i target = Vector2i(playerPos.x / mapTileSize.x,playerPos.y / mapTileSize.y);

	//gets the enemy's position in map coordinates
	Vector2i source = Vector2i(enemyPos.x / mapTileSize.x, enemyPos.y / mapTileSize.y);

	//The open list represents all possible tiles that enemy can move next
	std::list<shared_ptr<Node>> OpenList;

	//Add the enemy's position as the first point
	OpenList.push_back(make_shared<Node>(source,nullptr,0,Math::DistanceBetweenVectors(target,source)));

	//The closed list represents tiles that the algorithm has already tried
	std::list<shared_ptr<Node>> ClosedList;

	//The target node represents the node that is equal to the player's position. If this is no longer nullptr, the algorithm has succeeded
	shared_ptr<Node> targetNode = nullptr;

	//Repeat while there are valid tiles in the open list
	while (OpenList.size() > 0)
	{
		//Sort the open list by the lowest F number. The lower the number, the closer it is to the target
		OpenList.sort([](std::shared_ptr<Node>& A, std::shared_ptr<Node>& B)
			{
				return A->f > B->f;
			});

		//Get the tile with the lowest F number
		auto currentTile = OpenList.back();

		//If the tile's position is the same as the player's, we have found a path to the player
		if (currentTile->MapPosition == target)
		{
			//Set it as the target node
			targetNode = currentTile;
			//Break out of the loop
			break;
		}
		//Remove the tile from the open list
		OpenList.pop_back();

		//Add it to the closed list
		ClosedList.push_back(currentTile);

		//Loop over all the neighbors of the tile
		for (auto& neighbor : GetNeighboringTiles(GetMap(), currentTile->MapPosition))
		{
			//Get the neighboring tile
			BackgroundTile* tile = get<0>(neighbor);
			//Get the neighboring tile's position
			Vector2i tilePosition = get<1>(neighbor);

			//Create a new node with the neighbor's information
			shared_ptr<Node> newNode = make_shared<Node>(tilePosition, currentTile,currentTile->n + 1,Math::DistanceBetweenVectors(tilePosition,target));

			//If the neighboring tile is not null and is not collidable
			if (tile != nullptr && !tile->IsCollidable())
			{
				bool notInList = true; //Used to check if the node is in either the open or closed lists

				//Loop over all the nodes in the open list
				for (auto& node : OpenList)
				{
					//If the neighboring node is already in the open list
					if (node->MapPosition == tilePosition)
					{
						//The node is in the list
						notInList = false;
						//Break out
						break;
					}
				}
				//If the Open list has been checked and the node is not in there
				if (notInList)
				{
					//Loop over all the nodes in the closed list
					for (auto& node : ClosedList)
					{
						//If the neighboring node is already in the closed list
						if (node->MapPosition == tilePosition)
						{
							//The node is in the list
							notInList = false;
							//Break out
							break;
						}
					}
				}

				//If the node is not in either the open or closed lists
				if (notInList)
				{
					//Add it to the open list
					OpenList.push_back(newNode);
				}
			}
			//Continue to the next neighbor
		}
		//Continue to the next tile in the open list
	}
	//If the target was not found
	if (targetNode == nullptr)
	{
		//Return an empty path
		return PathResult();
	}

	//Used to iterate through the node structure
	shared_ptr<Node> nodePath = targetNode;

	//Stores the final path the enemy will take to get to the player
	std::list<sf::Vector2f> Path;

	//Repeat untill there are no more nodes left to iterate
	while (nodePath != nullptr)
	{
		//Get the position of the node
		auto position = nodePath->MapPosition;
		//Convert it to world coordinates and add it to the path list
		Path.emplace_front(static_cast<float>(position.x * mapTileSize.x),static_cast<float>(position.y * mapTileSize.y));
		//Go to the previous node in the node structure
		nodePath = nodePath->PreviousNode;
	}

	//Return the final path
	return PathResult(std::move(Path), true);
}

//Generates a path asynchronously. You will know when it's done when the "done" field is set to true
std::shared_ptr<PathAsyncResult> Enemy::GeneratePathToPlayerAsync()
{
	//Create a new async result object
	std::shared_ptr<PathAsyncResult> result = make_shared<PathAsyncResult>();

	//Add a new function to the thread pool
	ThreadPool::AddToQueue([result,enemy = this]() 
		{
			//Call the algorithm and get the results
			auto finalResult = enemy->GeneratePathToPlayer();
			
			//Set the path results of the async result
			result->result = std::move(finalResult);
			//Notify that the calculation is complete
			result->done = true;
		});

	//Return the asynchronous result
	return result;
}

//Gets all currently spawned enemies
const std::vector<Enemy*>& Enemy::GetAllEnemies()
{
	return ObjectManager<Enemy>::GetObjectList();
}

//Gets the mutex that controls the enemy list. Used to prevent data races
std::recursive_mutex& Enemy::GetEnemyListMutex()
{
	return ObjectManager<Enemy>::GetMutex();
}

//Constructs a new enemy
Enemy::Enemy(const WorldMap& map, bool enableCollision) :
	AnimatedEntity(map,true,1.0f / 12.0f,enableCollision)
{

}


//Constructs an empty results object
PathResult::PathResult()
{

}

//Constructs a new results object with the specified parameters
PathResult::PathResult(const std::list<sf::Vector2f>& path, bool success) :
	Path(path),
	Success(success)
{

}

//Constructs a new results object with the specified parameters
PathResult::PathResult(std::list<sf::Vector2f>&& path, bool success) :
	Path(std::move(path)),
	Success(success)
{
}
