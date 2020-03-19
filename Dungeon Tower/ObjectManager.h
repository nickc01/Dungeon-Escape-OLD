#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <type_traits>
#include <exception>
#include <string>
#include <mutex>





template<typename objectType>
class ObjectManager
{

	static std::vector<objectType*> list;
	static std::recursive_mutex listLock;

	bool inList;

	static void AddToList(objectType* object)
	{
		auto lockObject = std::unique_lock<std::recursive_mutex>(listLock);
		list.push_back(object);
	}
	static void RemoveFromList(objectType* object)
	{
		auto lockObject = std::unique_lock<std::recursive_mutex>(listLock);
		list.erase(std::remove(begin(list),end(list),object),end(list));
	}

protected:
	static std::vector<objectType*>& GetEventList()
	{
		return list;
	}

	void SetActive(bool enable)
	{
		if (inList != enable)
		{
			inList = enable;
			if (inList)
			{
				AddToList(static_cast<objectType*>(this));
			}
			else
			{
				RemoveFromList(static_cast<objectType*>(this));
			}
		}
	}

public:

	ObjectManager() : ObjectManager(true) {}

	ObjectManager(bool enabled) :
		inList(enabled)
	{
		if (enabled)
		{
			AddToList(static_cast<objectType*>(this));
		}
	}

	//Copy Constructor
	ObjectManager(const ObjectManager<objectType>& source)
	{
		SetActive(source.inList);
	}

	//Move Constructor
	ObjectManager(ObjectManager<objectType>&& source)
	{
		SetActive(source.inList);
		source.SetActive(false);
	}

	//Copy Assignment
	ObjectManager<objectType>& operator=(const ObjectManager<objectType>& source) noexcept
	{
		SetActive(source.ineList);
		return *this;

	}
	//Move Assignment
	ObjectManager<objectType>& operator=(ObjectManager<objectType>&& source) noexcept
	{
		SetActive(source.inList);
		source.SetActive(false);
		return *this;
	}

	virtual ~ObjectManager()
	{
		SetActive(false);
		inList = false;
	}

	//Retrieves the mutex that is used when adding and removing objects to the object list
	//You can manually lock this whenever you want to ensure the list stays consistent between threads
	static std::recursive_mutex& GetMutex()
	{
		return listLock;
	}

};

template<typename objectType>
std::vector<objectType*> ObjectManager<objectType>::list{};

template<typename objectType>
std::recursive_mutex ObjectManager<objectType>::listLock{};