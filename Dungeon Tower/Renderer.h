#pragma once

#include "Displayable.h"

namespace Renderer
{
	extern Vector2 CameraPosition;


	void AddDisplayable(const Displayable* sprite);

	void RemoveDisplayable(const Displayable* sprite);
}

