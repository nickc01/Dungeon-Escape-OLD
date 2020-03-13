
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

/*template<typename charType>
using MultiString = std::basic_string<charType, std::char_traits<charType>, std::allocator<charType>>;

template<typename charType>
using MultiStringStream = std::basic_stringstream<charType, std::char_traits<charType>, std::allocator<charType>>;



void WaitForInput();
void PrintTitleScreen();

void PrintInstructions();*/

int RandomNumber(int minRange, int maxRange);