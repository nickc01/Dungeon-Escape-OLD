#pragma once

#include <SFML/Graphics.hpp>

#include "ResourceFile.h"
#include <string>


class ResourceTexture
{
	ResourceFile resource;
	sf::Texture texture;

public:

	ResourceTexture(int resourceID, const std::string resourceClass = "PNG");
	ResourceTexture();

	const ResourceFile& GetResourceFile() const;

	const sf::Texture& GetTexture() const;

	sf::Texture& GetTexture();
};

