#pragma once

#include <SFML/Graphics.hpp>

#include "ResourceFile.h"

class ResourceFont
{
	ResourceFile resource;
	sf::Font font;

public:

	ResourceFont(int resourceID);
	ResourceFont();

	const ResourceFile& GetResourceFile() const;

	const sf::Font& GetFont() const;

	sf::Font& GetFont();
};

