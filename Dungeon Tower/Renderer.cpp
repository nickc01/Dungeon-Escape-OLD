#include "Renderer.h"
#include <vector>
#include <algorithm>

using namespace std;

vector<const Displayable*> DisplayableList;


Vector2 Renderer::CameraPosition = Vector2(0, 0);


void Renderer::AddDisplayable(const Displayable* sprite)
{
	DisplayableList.push_back(sprite);
	/*sort(begin(DisplayableList), end(DisplayableList), [](const Displayable* A, const Displayable* B)
		{
			int ADisplayLayer = A->GetDisplayLayer();
			int BDisplayLayer = B->GetDisplayLayer();
			return ADisplayLayer < BDisplayLayer;
		});*/
}

void Renderer::RemoveDisplayable(const Displayable* sprite)
{
	DisplayableList.erase(std::remove(begin(DisplayableList),end(DisplayableList),sprite),end(DisplayableList));
}

