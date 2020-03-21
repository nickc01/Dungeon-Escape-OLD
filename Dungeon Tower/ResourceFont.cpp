#include "ResourceFont.h"

ResourceFont::ResourceFont(int resourceID) :
	resource(resourceID, RT_RCDATA)
{
	auto data = resource.GetData();
	bool success = font.loadFromMemory(data, resource.GetDataSize());
}

ResourceFont::ResourceFont()
{

}

const ResourceFile& ResourceFont::GetResourceFile() const
{
	return resource;
}

const sf::Font& ResourceFont::GetFont() const
{
	return font;
}

sf::Font& ResourceFont::GetFont()
{
	return font;
}
