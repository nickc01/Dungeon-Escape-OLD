#include "ResourceTexture.h"

ResourceTexture::ResourceTexture(int resourceID, const std::string resourceClass) :
	resource(resourceID,resourceClass)
{
	texture.loadFromMemory(resource.GetData(), resource.GetDataSize());
}

ResourceTexture::ResourceTexture()
{

}

const ResourceFile& ResourceTexture::GetResourceFile() const
{
	return resource;
}

const sf::Texture& ResourceTexture::GetTexture() const
{
	return texture;
}

sf::Texture& ResourceTexture::GetTexture()
{
	return texture;
}
