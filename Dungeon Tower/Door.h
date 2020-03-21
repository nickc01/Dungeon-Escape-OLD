#pragma once
#include "Renderable.h"
#include "ResourceTexture.h"


class Door : public Renderable
{
	static Door* currentDoor;
	static ResourceTexture doorTexture;

	sf::Sprite doorSprite;

	// Inherited via Renderable
	virtual void Render(sf::RenderWindow& window) override;

public:

	Door(sf::Vector2f doorPosition);

	static Door* GetDoor();

	const sf::Sprite& GetSprite() const;


	Door(const Door&) = delete;
	Door(Door&&) = delete;

	Door& operator=(const Door&) = delete;
	Door& operator=(Door&&) = delete;

	virtual ~Door();
};

