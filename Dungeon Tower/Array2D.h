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

	const ElementType& operator[](Vector2 position) const
	{
		return Get(std::get<0>(position), std::get<1>(position));
	}

	ElementType& operator[](Vector2 position)
	{
		return Get(std::get<0>(position), std::get<1>(position));
	}


};

