#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources

using namespace sf; //Prevents me from having to type sf everywhere

//Constructs and Texture the font into memory
ResourceTexture::ResourceTexture(int resourceID, const std::string resourceClass) :
	resource(resourceID,resourceClass) //Load the resource
{
	//Load the texture from the resource memory
	texture.loadFromMemory(resource.GetData(), resource.GetDataSize());
}

//Constructs a blank ResourceTexture
ResourceTexture::ResourceTexture() {}

//Gets the raw resource file that loaded the Texture
const ResourceFile& ResourceTexture::GetResourceFile() const
{
	return resource;
}

//Gets the texture that was loaded
const Texture& ResourceTexture::GetTexture() const
{
	return texture;
}

//Gets the texture that was loaded
Texture& ResourceTexture::GetTexture()
{
	return texture;
}
