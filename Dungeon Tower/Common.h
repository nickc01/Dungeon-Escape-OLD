
#include <SFML/Graphics.hpp>
#include <tuple>
#include <string>
#include <iostream>
#include <limits>

#include "ResourceTexture.h"

#include "resource.h"

namespace Common
{
	namespace Sprites
	{
		extern sf::Sprite blankTile;

		extern sf::Sprite topLeftPiece;
		extern sf::Sprite topRightPiece;
		extern sf::Sprite bottomLeftPiece;
		extern sf::Sprite bottomRightPiece;
		extern sf::Sprite topPiece;
		extern sf::Sprite bottomPiece;
		extern sf::Sprite rightPiece;
		extern sf::Sprite leftPiece;

	}

	namespace Textures
	{
		extern ResourceTexture blankTexture;
		extern ResourceTexture cornerPiece;
		extern ResourceTexture sidePiece;

	}

	extern sf::RenderWindow window;

	void CreateSprites();
}


int RandomNumber(int minRange, int maxRange);