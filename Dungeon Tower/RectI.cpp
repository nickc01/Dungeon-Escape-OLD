#include "RectI.h"

using namespace std;

RectI::RectI(Vector2I Center, Vector2I dimensions)
{
	int leftDimensions = dimensions.x / 2;
	int rightDimensions = dimensions.x - leftDimensions;

	int upDimensions = dimensions.y / 2;
	int downDimensions = dimensions.y - upDimensions;

	topHeight = upDimensions;
	bottomHeight = downDimensions;
	leftWidth = leftDimensions;
	rightWidth = rightDimensions;


	Left = Center.x - leftDimensions;
	Right = Center.x + rightDimensions;
	Top = Center.y + upDimensions;
	Bottom = Center.y - downDimensions;

	this->Center = Center;
}

bool RectI::Intersects(const RectI& B) const
{
	if (Left > B.Right || Right < B.Left || Top < B.Bottom || Bottom > B.Top)
	{
		return false;
	}
	else
	{
		return true;
	}
}

Vector2I RectI::BottomLeft() const
{
	return Vector2I(Left, Bottom);
}

Vector2I RectI::TopRight() const
{
	return Vector2I(Right, Top);
}

int RectI::TopHeight() const
{
	return topHeight;
}

int RectI::BottomHeight() const
{
	return bottomHeight;
}

int RectI::LeftWidth() const
{
	return leftWidth;
}

int RectI::RightWidth() const
{
	return rightWidth;
}

int RectI::GetWidth() const
{
	return leftWidth + rightWidth;
}

int RectI::GetHeight() const
{
	return topHeight + bottomHeight;
}
