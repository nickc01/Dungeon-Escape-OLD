#pragma once

#include "Displayable.h"
#include <memory>
#include "BackgroundTile.h"
#include "Array2D.h"

namespace Renderer
{
	extern Vector2 CameraPosition;
	extern Array2D<std::shared_ptr<BackgroundTile>> BackgroundTiles;

	/*void AddDisplayable(const Displayable* sprite);

	void RemoveDisplayable(const Displayable* sprite);

	void SortDisplayables();*/

	void Render();
}

