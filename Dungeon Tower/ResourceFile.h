#pragma once

#include <string>
#include <memory>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//Loads the data from an embedded resource
//Credits : https://mklimenko.github.io/english/2018/06/23/embed-resources-msvc/
class ResourceFile
{
	size_t sizeBytes = 0;
	void* data = nullptr;

	HRSRC hResource = nullptr;
	HGLOBAL hMemory = nullptr;

public:

	ResourceFile(int resource_ID, const std::string resource_class);
	ResourceFile(int resource_ID, decltype(RT_RCDATA) test);
	ResourceFile();

	size_t GetDataSize() const;
	void* GetData() const;
};

