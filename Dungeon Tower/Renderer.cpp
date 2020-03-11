#include "Renderer.h"
#include <vector>
#include <algorithm>
#include <map>
#include "Array2D.h"
#include <thread>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

//vector<const Displayable*> DisplayableList;

//vector<const Displayable*> DisplayableList;

//map<int, Array2D<CHAR_INFO>> DisplayMaps;


Vector2 Renderer::CameraPosition = Vector2(0, 0);

Array2D<shared_ptr<BackgroundTile>> Renderer::BackgroundTiles;

/*void RenderListPartial(int begin, int end)
{
	for (int i = begin; i < end; i++)
	{
		DisplayableList[i]->Render();
	}
}*/

/*void Renderer::AddDisplayable(const Displayable* sprite)
{
	DisplayableList.push_back(sprite);
}

void Renderer::RemoveDisplayable(const Displayable* sprite)
{
	DisplayableList.erase(std::remove(begin(DisplayableList),end(DisplayableList),sprite),end(DisplayableList));
}

void Renderer::SortDisplayables()
{
	sort(begin(DisplayableList), end(DisplayableList), [](const Displayable* A, const Displayable* B)
	{
		int ADisplayLayer = A->GetDisplayLayer();
		int BDisplayLayer = B->GetDisplayLayer();
		return ADisplayLayer < BDisplayLayer;
	});
}*/

void Renderer::Render()
{
	auto windowSize = Console::GetConsoleWindowSize();

	int width = get<0>(windowSize);
	int height = get<1>(windowSize);

	int CamX = get<0>(CameraPosition);
	int CamY = get<1>(CameraPosition);

	int Left = CamX - (width / 2);
	int Bottom = CamY - (height / 2);

	for (int x = Left; x < Left + width; x++)
	{
		for (int y = Bottom; y < Bottom + height; y++)
		{
			auto tile = BackgroundTiles[{x, y}];
			if (tile == nullptr)
			{
				Console::SetColorAtPosition(x - Left,y - Bottom,Black,Black);
				Console::DrawCharacter(x - Left, y - Bottom, L' ');
			}
			else
			{
				tile->Render();
			}
		}
	}

	/*vector<thread> threads;

	int partitionSize = DisplayableList.size() / 8;

	for (int i = 0; i < 8; i++)
	{
		threads.push_back(thread(RenderListPartial,i * partitionSize,(i + 1) * partitionSize));
	}

	for (auto& thread : threads)
	{
		thread.join();
	}*/
	/*for (auto displayable : DisplayableList)
	{
		displayable->Render();
	}*/
}

