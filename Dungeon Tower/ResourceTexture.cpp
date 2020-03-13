#include "ResourceTexture.h"

using namespace sf;

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

const Texture& ResourceTexture::GetTexture() const
{
	return texture;
}

Texture& ResourceTexture::GetTexture()
{
	return texture;
}
