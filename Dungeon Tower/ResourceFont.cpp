#include "ResourceFont.h" //Contains the ResourceFont class, which is used to load font resources

//Constructs and loads the font into memory
ResourceFont::ResourceFont(int resourceID) :
	resource(resourceID, RT_RCDATA) //Load the resource
{
	//Load the font from the resource memory
	font.loadFromMemory(resource.GetData(), resource.GetDataSize());
}

//Loads a blank ResourceFont
ResourceFont::ResourceFont() {}

//Gets the raw resource file that loaded the font
const ResourceFile& ResourceFont::GetResourceFile() const
{
	return resource;
}

//Gets the font that was loaded
const sf::Font& ResourceFont::GetFont() const
{
	return font;
}

//Gets the font that was loaded
sf::Font& ResourceFont::GetFont()
{
	return font;
}
