#pragma once

#include <SFML/Graphics.hpp>
#include <array>

namespace Math
{
	template<typename NumType, typename TType = NumType>
	NumType Lerp(NumType A, NumType B, TType t)
	{
		return A + ((B - A) * t);
	}

	template<typename NumType, typename TType = NumType>
	sf::Vector2<NumType> VectorLerp(sf::Vector2<NumType> A, sf::Vector2<NumType> B, TType t)
	{
		return sf::Vector2<NumType>(Lerp(A.x,B.x,t),Lerp(A.y,B.y,t));
	}

	template<typename VectorType, typename ReturnType = float>
	ReturnType DistanceBetweenVectors(sf::Vector2<VectorType> A, sf::Vector2<VectorType> B)
	{
		//auto a = pow(B.x - A.x, 2);
		//auto b = pow(B.y - A.y, 2);

		return sqrt(pow(B.x - A.x, 2) + pow(B.y - A.y, 2));
	}

	template<typename VectorType>
	std::array<sf::Vector2<VectorType>, 2> VectorComponents(sf::Vector2<VectorType> vect)
	{
		std::array<sf::Vector2<VectorType>, 2> components{};

		components[0] = { vect.x,0 };
		components[1] = { 0,vect.y };

		return components;
	}

	template<typename VectorType, typename ReturnType = float>
	ReturnType GetVectorLength(sf::Vector2<VectorType> input)
	{
		return sqrt(pow(input.x, 2) + pow(input.y, 2));
	}

	//Normalizes the vector to a specified length
	template<typename VectorType>
	sf::Vector2<VectorType> NormalizeVector(sf::Vector2<VectorType> input, VectorType length = static_cast<VectorType>(1))
	{
		constexpr VectorType Zero = static_cast<VectorType>(0);

		float VectorLength = GetVectorLength(input);
		if (VectorLength == Zero)
		{
			return sf::Vector2<VectorType>(Zero,Zero);
		}
		return sf::Vector2<VectorType>((input.x / VectorLength) * length,(input.y / VectorLength) * length);
	}


}

