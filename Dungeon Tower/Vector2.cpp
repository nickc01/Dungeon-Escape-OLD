#include "Vector2.h"

Vector2 operator+(Vector2 A, Vector2 B)
{
	return Vector2(std::get<0>(A) + std::get<0>(B), std::get<1>(A) + std::get<1>(B));
}

Vector2 operator-(Vector2 A, Vector2 B)
{
	return Vector2(std::get<0>(A) - std::get<0>(B), std::get<1>(A) - std::get<1>(B));
}

Vector2 operator*(Vector2 A, int scalar)
{
	return Vector2(std::get<0>(A) * scalar, std::get<1>(A) * scalar);
}

Vector2 operator/(Vector2 A, int scalar)
{
	return Vector2(std::get<0>(A) / scalar, std::get<1>(A) / scalar);
}

Vector2 operator+=(Vector2& A, Vector2 B)
{
	return A = A + B;
}

Vector2 operator-=(Vector2& A, Vector2 B)
{
	return A = A - B;
}

Vector2 operator*=(Vector2& A, int scalar)
{
	return A = A * scalar;
}

Vector2 operator/=(Vector2& A, int scalar)
{
	return A = A / scalar;
}
