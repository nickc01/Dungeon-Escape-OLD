#pragma once
#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <vector> //Contains std::vector for storing objects in an array


//Represents a two dimensional array of objects
template<typename ElementType>
class Array2D
{
	//The flattened 2D array
	std::vector<ElementType> FlatArray;

	int width; //The width of the 2D array
	int height; //The height of the 2D array

	int offsetX = 0;
	int offsetY = 0;

public:

	//Constructs a new 2D array. This will also construct any new objects that will be in the array
	template<typename... ConstructorParams>
	Array2D(int width, int height, ConstructorParams&&... params) : width(width), height(height)
	{
		//Loop over each index in the array
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				//Construct a new object at that index
				FlatArray.emplace_back(params...);
			}
		}
	}

	//Constructs a new 2D array. This will also construct any new objects that will be in the array
	template<typename... ConstructorParams>
	Array2D(sf::Vector2<int> Dimensions, ConstructorParams&&... params) : Array2D(Dimensions.x,Dimensions.y,params...)
	{

	}

	//Constructs a blank 2D array
	Array2D() : Array2D(0,0)
	{

	}

	//Gets the width of the 2D array
	int GetWidth() const
	{
		return width;
	}

	//Gets the height of the 2D array
	int GetHeight() const
	{
		return height;
	}

	//Gets the positional offset of the 2D array
	sf::Vector2<int> GetOffset() const
	{
		return sf::Vector2<int>(offsetX,offsetY);
	}

	//Sets the positional offset of the 2D array
	void SetOffset(sf::Vector2<int> offset)
	{
		offsetX = offset.x;
		offsetY = offset.y;
	}

	//Check if the specified index is within the bounds of the array
	bool WithinBounds(int x, int y) const
	{
		//Subtract the positional offset
		x -= offsetX;
		y -= offsetY;
		return !(x < 0 || x >= width || y < 0 || y >= height);
	}

	//Gets an object at the specified index
	const ElementType& Get(int x, int y) const
	{
		//Subtract the positional offset
		x -= offsetX;
		y -= offsetY;

		//Return the object
		return FlatArray[x + (width * y)];
	}

	//Gets an object at the specified index
	ElementType& Get(int x, int y)
	{
		//Subtract the positional offset
		x -= offsetX;
		y -= offsetY;

		//Return the object
		return FlatArray[x + (width * y)];
	}

	//Gets an object at the specified index, without taking into account the positional offset
	const ElementType& GetRelative(int x, int y) const
	{
		return FlatArray[x + (width * y)];
	}

	//Gets an object at the specified index, without taking into account the positional offset
	ElementType& GetRelative(int x, int y)
	{
		return FlatArray[x + (width * y)];
	}

	//Sets an object at the specified index
	void Set(ElementType element, int x, int y)
	{
		//Subtract the positional offset
		x -= offsetX;
		y -= offsetY;

		//Set the object
		FlatArray[x + (width * y)] = element;
	}

	//Sets an object at the specified index
	void Set(ElementType element, sf::Vector2<int> position)
	{
		Set(std::forward(element), position.x, position.y);
	}

	//Gets an object at the specified index
	const ElementType& operator[](sf::Vector2<int> position) const
	{
		return Get(position.x, position.y);
	}

	//Gets an object at the specified index
	ElementType& operator[](sf::Vector2<int> position)
	{
		return Get(position.x, position.y);
	}
};

