#pragma once


#include <SFML/Graphics.hpp>
#include <tuple>
#include <string>
#include <iostream>
#include <limits>
#include "Direction.h"

#include "ResourceTexture.h"

#include "resource.h"

namespace Common
{
	namespace Sprites
	{
		extern sf::Sprite centerPiece1;
		extern sf::Sprite centerPiece2;

		extern sf::Sprite topJointPiece;
		extern sf::Sprite bottomJointPiece;
		extern sf::Sprite leftJointPiece;
		extern sf::Sprite rightJointPiece;

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
		extern ResourceTexture centerPiece1;
		extern ResourceTexture centerPiece2;

		extern ResourceTexture topJointPiece;
		extern ResourceTexture bottomJointPiece;
		extern ResourceTexture leftJointPiece;
		extern ResourceTexture rightJointPiece;

		extern ResourceTexture topLeftPiece;
		extern ResourceTexture topRightPiece;
		extern ResourceTexture bottomLeftPiece;
		extern ResourceTexture bottomRightPiece;
		extern ResourceTexture topPiece;
		extern ResourceTexture bottomPiece;
		extern ResourceTexture rightPiece;
		extern ResourceTexture leftPiece;

	}

	extern sf::RenderWindow MainWindow;

	void CreateSprites();

	sf::Sprite GetCenterSprite();
	sf::Sprite GetJointPiece(Direction source, Direction to);
	sf::Sprite GetSideSprite(Direction side);
	sf::Sprite GetCornerSprite(Direction A, Direction B);

	bool SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, sf::Vector2f scaleFactor = {-1.0f,-1.0f});
}

void RefreshWindow(sf::RenderWindow& window = Common::MainWindow);
void CenterCamera(sf::Vector2f center, sf::RenderWindow& window = Common::MainWindow);
int RandomNumber(int minRange, int maxRange);