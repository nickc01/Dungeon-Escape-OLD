#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include "Direction.h"
#include "Entity.h"

namespace Math
{
	constexpr double PI = 3.14159265358979323846;


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

	template<typename VectorType, typename ReturnType = float>
	ReturnType VectorToDegrees(sf::Vector2<VectorType> input)
	{
		sf::Vector2<VectorType> normalized = NormalizeVector(input);

		return ((atan2(normalized.y, normalized.x) / PI) * -180.0f) + 90.0f;
	}

	template<typename VectorType>
	Direction ApproxDirectionOfVector(sf::Vector2<VectorType> input)
	{

		float degrees = VectorToDegrees(input);

		if (degrees < 0)
		{
			degrees += 360.0f;
		}

		if (degrees >= 45 && degrees < 135)
		{
			return Direction::Right;
		}
		else if (degrees >= 135 && degrees < 225)
		{
			return Direction::Down;
		}
		else if (degrees >= 225 && degrees < 315)
		{
			return Direction::Left;
		}
		else
		{
			return Direction::Up;
		}
	}

	template<typename RectType>
	bool RectsIntersect(sf::Rect<RectType> A, sf::Rect<RectType> B)
	{
		auto leftA = A.left;
		auto rightA = A.left + A.width;
		auto topA = A.top;
		auto bottomA = A.top - A.height;

		auto leftB = B.left;
		auto rightB = B.left + B.width;
		auto topB = B.top;
		auto bottomB = B.top - B.height;

		if (leftA < rightB && rightA > leftB&& topA > bottomB&& bottomA < topB)
		{
			return true;
		}
		return false;
	}

	bool SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B);
	bool EntitiesIntersect(const Entity& A, const Entity& B);
}

