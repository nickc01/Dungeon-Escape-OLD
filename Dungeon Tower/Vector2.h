#pragma once
#include <SFML/Graphics.hpp>

#include <tuple>
//#include <SFML/Graphics.hpp>

using Vector2 = sf::Vector2f;
using Vector2I = sf::Vector2i;

/*template<typename type>
sf::Vector2<type> operator/(sf::Vector2<type> A, int scalar)
{
	return sf::Vector2<type>(A.x / scalar, A.y / scalar);
}

template<typename type>
sf::Vector2<type> operator*(sf::Vector2<type> A, int scalar)
{
	return sf::Vector2<type>(A.x * scalar, A.y * scalar);
}

template<typename type>
sf::Vector2<type> operator/=(sf::Vector2<type>& A, int scalar)
{
	return A = A / scalar;
}

template<typename type>
sf::Vector2<type> operator*=(sf::Vector2<type>& A, int scalar)
{
	return A = A * scalar;
}*/

namespace VectorMath
{
	template<typename type>
	sf::Vector2<type> SmallestParts(sf::Vector2<type> A, sf::Vector2<type> B)
	{
		return sf::Vector2<type>(A.x < B.x ? A.x : B.x, A.y < B.y ? A.y : B.y);
	}

	template<typename type>
	sf::Vector2<type> GreatestParts(sf::Vector2<type> A, sf::Vector2<type> B)
	{
		return sf::Vector2<type>(A.x > B.x ? A.x : B.x, A.y > B.y ? A.y : B.y);
	}
}


