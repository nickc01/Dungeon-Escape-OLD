#include "Renderer.h"
#include <vector>
#include <algorithm>
#include <map>
#include "Array2D.h"
#include <thread>

/*#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace std;

Vector2 Renderer::CameraPosition = Vector2(0, 0);

Array2D<shared_ptr<BackgroundTile>> Renderer::BackgroundTiles;


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
}*/
