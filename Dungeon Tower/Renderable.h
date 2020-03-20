#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "ObjectManager.h"
#include <atomic>


class Renderable : public ObjectManager<Renderable>
{
	int renderLayer = 0;
	static std::atomic<bool> listUpdated;


public:

	Renderable(bool renderingEnabled = false);

	static const std::vector<Renderable*>& GetRenderables();

	void EnableRendering(bool enabled = true);

	int GetRenderLayer() const;
	void SetRenderLayer(int newLayer);

	virtual void Render(sf::RenderWindow& window) = 0;


	virtual ~Renderable();
};

