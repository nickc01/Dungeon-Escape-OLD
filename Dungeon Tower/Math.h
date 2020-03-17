#pragma once

#include <SFML/Graphics.hpp>

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
}

