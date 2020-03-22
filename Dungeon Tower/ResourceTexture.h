#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering

#include "ResourceFile.h" //Contains the ResourceFile class, which is used to load embedded resources into memory
#include <string> //Contains std::string

//Loads a SFML Texture from a resource
class ResourceTexture
{
	ResourceFile resource; //The resource to load
	sf::Texture texture; //The loaded Texture

public:

	//Constructs and Texture the font into memory
	ResourceTexture(int resourceID, const std::string resourceClass = "PNG");
	//Constructs a blank ResourceTexture
	ResourceTexture();

	//Gets the raw resource file that loaded the Texture
	const ResourceFile& GetResourceFile() const;

	//Gets the texture that was loaded
	const sf::Texture& GetTexture() const;

	//Gets the texture that was loaded
	sf::Texture& GetTexture();
};

