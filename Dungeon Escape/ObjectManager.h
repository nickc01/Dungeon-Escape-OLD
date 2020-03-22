#pragma once

#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <algorithm> //Contains many commonly used algorithms such as std::sort and std::finds>
#include <string> //Contains std::string
#include <mutex> //Contains std::mutex and std::recursive_mutex for resource locking and to prevent data races




//Used to store a collection of all instantiated objects
template<typename objectType,typename MutexType = std::recursive_mutex>
class ObjectManager
{

	static std::vector<objectType*> list; //The list of all instantiated objects
	static MutexType listLock; //The lock for the object list

	bool inList; //Whether this object is in the list or not

	//Adds the object to the list
	static void AddToList(objectType* object)
	{
		//Lock the list mutex
		auto lockObject = std::unique_lock<MutexType>(listLock);
		//Add the object to the list
		list.push_back(object);
	}
	//Removes the object from the list
	static void RemoveFromList(objectType* object)
	{
		//Lock the list mutex
		auto lockObject = std::unique_lock<MutexType>(listLock);
		//Remove the object from the list
		list.erase(std::remove(begin(list),end(list),object),end(list));
	}

protected:
	//Gets the object list
	static std::vector<objectType*>& GetObjectList()
	{
		return list;
	}

	//Sets whether an object is active in the list or not
	void SetActive(bool enable)
	{
		//If the passed in value is different
		if (inList != enable)
		{
			inList = enable;
			//If the object is to be in the list and enabled
			if (inList)
			{
				//Add it to the list
				AddToList(static_cast<objectType*>(this));
			}
			//If the object is to not be in the list and disabled
			else
			{
				//Remove it from the list
				RemoveFromList(static_cast<objectType*>(this));
			}
		}
	}

public:

	//Constructs a new object manager
	ObjectManager() : ObjectManager(true) {}

	//Constructs a new object manager
	ObjectManager(bool enabled) :
		inList(enabled)
	{
		//If the object is to be enabled
		if (enabled)
		{
			//Add it to the list
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

	//Destructor
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

template<typename objectType,typename MutexType>
std::vector<objectType*> ObjectManager<objectType,MutexType>::list{};  //The list of all instantiated objects

template<typename objectType,typename MutexType>
MutexType ObjectManager<objectType,MutexType>::listLock{}; //The lock for the object list