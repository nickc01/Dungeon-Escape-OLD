#include "Rect.h"

using namespace std;

Rect::Rect(Vector2 Center, Vector2 dimensions)
{
	int leftDimensions = get<0>(dimensions) / 2;
	int rightDimensions = get<0>(dimensions) - leftDimensions;

	int upDimensions = get<1>(dimensions) / 2;
	int downDimensions = get<1>(dimensions) - upDimensions;


	Left = get<0>(Center) - leftDimensions;
	Right = get<0>(Center) + rightDimensions;
	Top = get<1>(Center) + upDimensions;
	Bottom = get<1>(Center) - downDimensions;
}

bool Rect::Intersects(const Rect& B) const
{
	if (Left > B.Right || Right < B.Left || Top < B.Bottom || Bottom > B.Top)
	{
		return false;
	}
	else
	{
		return true;
	}
	//bool left = Left <= B.Right;
	//bool top = Top >= B.Bottom;
	//bool right = Right >= B.Left;
	//bool bottom = Bottom <= B.Top;

	//return left || top || right || bottom;

	//return Left <= B.Right || Top >= B.Bottom || Right >= B.Left || Bottom <= B.Top;
}
