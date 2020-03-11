#include "Vector2.h"

using namespace std;

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

Vector2 VectorMath::SmallestParts(Vector2 A, Vector2 B)
{
	int Ax = get<0>(A);
	int Ay = get<1>(A);
	int Bx = get<0>(B);
	int By = get<1>(B);


	return Vector2(Ax < Bx ? Ax : Bx,Ay < By ? Ay : By);
}

Vector2 VectorMath::GreatestParts(Vector2 A, Vector2 B)
{
	int Ax = get<0>(A);
	int Ay = get<1>(A);
	int Bx = get<0>(B);
	int By = get<1>(B);


	return Vector2(Ax > Bx ? Ax : Bx, Ay > By ? Ay : By);
}
