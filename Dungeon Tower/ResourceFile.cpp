#include <strsafe.h>
#include "ResourceFile.h"
#include <exception>
#include <string>

using namespace std;


ResourceFile::ResourceFile(int resource_ID, const std::string resource_class)
{
    hResource = FindResourceA(nullptr, MAKEINTRESOURCEA(resource_ID), resource_class.c_str());
    if (hResource == 0)
    {
        throw std::exception("Unable to load resource");
    }
    hMemory = LoadResource(nullptr, hResource);

    sizeBytes = SizeofResource(nullptr, hResource);
    if (hMemory == 0)
    {
        throw std::exception("Unable to load resource");
    }
    data = LockResource(hMemory);
}

ResourceFile::ResourceFile()
{

}

size_t ResourceFile::GetDataSize() const
{
    return sizeBytes;
}

void* ResourceFile::GetData() const
{
    return data;
}
