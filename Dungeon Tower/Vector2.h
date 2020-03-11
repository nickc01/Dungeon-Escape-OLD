#pragma once

#include <tuple>

using Vector2 = std::tuple<int, int>;


Vector2 operator+(Vector2 A, Vector2 B);

Vector2 operator-(Vector2 A, Vector2 B);

Vector2 operator*(Vector2 A, int scalar);

Vector2 operator/(Vector2 A, int scalar);

Vector2 operator+=(Vector2& A, Vector2 B);
Vector2 operator-=(Vector2& A, Vector2 B);
Vector2 operator*=(Vector2& A, int scalar);
Vector2 operator/=(Vector2& A, int scalar);

namespace VectorMath
{
	Vector2 SmallestParts(Vector2 A, Vector2 B);

	Vector2 GreatestParts(Vector2 A, Vector2 B);
}


