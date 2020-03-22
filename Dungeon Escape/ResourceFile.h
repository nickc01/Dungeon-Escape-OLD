#pragma once

#include <string> //Contains std::string
#include <memory> //Contains std::shared_ptr and std::unique_ptr

#define WIN32_LEAN_AND_MEAN //Prevents unnesessary libraries from being included with the windows.h header
#include <Windows.h> //Contains many essential functions for operating with the Windows OS, as well as giving us the ability to load resources


//Loads the data from an embedded resource
//Credits : https://mklimenko.github.io/english/2018/06/23/embed-resources-msvc/
class ResourceFile
{
	//The size f the resource
	size_t sizeBytes = 0;
	//A pointer to begining of the resource data
	void* data = nullptr;

	HRSRC hResource = nullptr; //The resource handle
	HGLOBAL hMemory = nullptr; //The ressource memory handle

public:

	//Constructs and loads the resource
	ResourceFile(int resource_ID, const std::string resource_class);
	//Constructs and loads the resource
	ResourceFile(int resource_ID, decltype(RT_RCDATA) test);
	//Constructs a blank resource file
	ResourceFile();

	//Gets the size of the resource
	size_t GetDataSize() const;
	//Gets the data pointer for the data
	void* GetData() const;
};

