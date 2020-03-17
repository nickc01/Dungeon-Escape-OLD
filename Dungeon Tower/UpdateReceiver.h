#pragma once

#include "ObjectManager.h"
#include <SFML/Graphics.hpp>


class UpdateReceiver : public ObjectManager<UpdateReceiver>
{
public:

	UpdateReceiver(bool enabled = true);

	static std::vector<UpdateReceiver*>& GetUpdatables();

	virtual void Update(sf::Time dt) = 0;
};

