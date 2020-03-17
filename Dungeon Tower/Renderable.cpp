#include "Renderable.h"
#include <algorithm>


std::atomic<bool> Renderable::listUpdated = false;

Renderable::Renderable() :
	ObjectManager<Renderable>(true)
{
	listUpdated = true;
}

const std::vector<Renderable*>& Renderable::GetRenderables()
{
	auto& renderables = ObjectManager<Renderable>::GetEventList();
	if (listUpdated)
	{

		std::unique_lock<std::recursive_mutex> listLock{ GetMutex() };
		std::sort(begin(renderables), end(renderables), [](Renderable* A, Renderable* B)
			{
				return A->renderLayer < B->renderLayer;
			});
		listUpdated = false;
	}
	return renderables;
}

void Renderable::EnableRendering(bool enabled)
{
	SetActive(enabled);
}


int Renderable::GetRenderLayer() const
{
	return renderLayer;
}

void Renderable::SetRenderLayer(int newLayer)
{
	if (renderLayer != newLayer)
	{
		listUpdated = true;
	}
	renderLayer = newLayer;
}

Renderable::~Renderable()
{
	listUpdated = true;
}
