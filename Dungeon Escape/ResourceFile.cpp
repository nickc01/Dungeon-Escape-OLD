#include <strsafe.h>
#include "ResourceFile.h" //Contains the ResourceFile class, which is used to load embedded resources into memory
#include <exception>
#include <string> //Contains std::string

using namespace std; //Prevents me from having to type std everywhere

//Constructs and loads the resource
ResourceFile::ResourceFile(int resource_ID, const std::string resource_class)
{
    //Find the resource if it exists
    hResource = FindResourceA(nullptr, MAKEINTRESOURCEA(resource_ID), resource_class.c_str());
    //If it does not exist
    if (hResource == 0)
    {
        //Throw an exception
        throw std::exception("Unable to load resource");
    }
    //Attempt to load the resource into memory
    hMemory = LoadResource(nullptr, hResource);

    //Attempt to get the resource's size
    sizeBytes = SizeofResource(nullptr, hResource);
    //If the resource failed to load into memory
    if (hMemory == 0)
    {
        //Throw an exception
        throw std::exception("Unable to load resource");
    }
    //Lock the resource and retrieve it's data
    data = LockResource(hMemory);
}

//Constructs and loads the resource
ResourceFile::ResourceFile(int resource_ID, decltype(RT_RCDATA) resource_Class)
{
    //Find the resource if it exists
    hResource = FindResource(nullptr, MAKEINTRESOURCE(resource_ID), resource_Class);
    //If it does not exist
    if (hResource == 0)
    {
        //Throw an exception
        throw std::exception("Unable to load resource");
    }
    //Attempt to load the resource into memory
    hMemory = LoadResource(nullptr, hResource);

    //Attempt to get the resource's size
    sizeBytes = SizeofResource(nullptr, hResource);
    //If the resource failed to load into memory
    if (hMemory == 0)
    {
        //Throw an exception
        throw std::exception("Unable to load resource");
    }
    //Lock the resource and retrieve it's data
    data = LockResource(hMemory);
}

//Constructs a blank resource file
ResourceFile::ResourceFile()
{

}

//Gets the size of the resource
size_t ResourceFile::GetDataSize() const
{
    return sizeBytes;
}

//Gets the data pointer for the data
void* ResourceFile::GetData() const
{
    return data;
}
