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

	Renderable();

	static const std::vector<Renderable*>& GetRenderables();

	void EnableRendering(bool enabled = true);

	int GetRenderLayer() const;
	void SetRenderLayer(int newLayer);

	virtual void Render(sf::RenderWindow& window) const = 0;


	virtual ~Renderable();
};

