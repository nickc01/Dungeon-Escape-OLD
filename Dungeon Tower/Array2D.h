#pragma once
#include <vector>


template<typename ElementType>
class Array2D
{
	std::vector<ElementType> FlatArray;

	int width;
	int height;

public:
	template<typename... ConstructorParams>
	Array2D(int width, int height, ConstructorParams&&... params) : width(width), height(height)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				FlatArray.emplace_back(params...);
			}
		}
	}

	template<typename... ConstructorParams>
	Array2D(Vector2I Dimensions, ConstructorParams&&... params) : Array2D(Dimensions.x,Dimensions.y,params...)
	{

	}

	Array2D() : Array2D(0,0)
	{

	}

	int GetWidth() const
	{
		return width;
	}

	int GetHeight() const
	{
		return height;
	}

	const ElementType& Get(int x, int y) const
	{
		return FlatArray[x + (width * y)];
	}

	ElementType& Get(int x, int y)
	{
		return FlatArray[x + (width * y)];
	}

	void Set(ElementType element, int x, int y)
	{
		FlatArray[x + (width * y)] = element;
	}

	void Set(ElementType element, Vector2I position)
	{
		Set(std::forward(element), position.x, position.y);
	}

	const ElementType& operator[](Vector2I position) const
	{
		return Get(position.x, position.y);
	}

	ElementType& operator[](Vector2I position)
	{
		return Get(position.x, position.y);
	}
};

