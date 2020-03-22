#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering

#include "ResourceFile.h" //Contains the ResourceFile class, which is used to load embedded resources into memory

//Loads a SFML font from a resource
class ResourceFont
{
	ResourceFile resource; //The resource to load
	sf::Font font; //The loaded font

public:

	//Constructs and loads the font into memory
	ResourceFont(int resourceID);
	//Loads a blank ResourceFont
	ResourceFont();

	//Gets the raw resource file that loaded the font
	const ResourceFile& GetResourceFile() const;

	//Gets the font that was loaded
	const sf::Font& GetFont() const;

	//Gets the font that was loaded
	sf::Font& GetFont();
};

