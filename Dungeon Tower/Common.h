#pragma once


#include <SFML/Graphics.hpp> //Contains many essential SFML classes and functions for rendering
#include <string> //Contains std::string
#include <memory> //Contains std::shared_ptr and std::unique_ptr
#include "Direction.h" //Contains the Direction Enum for specifying the direction

#include "ResourceTexture.h" //Contains the ResourceTexture class for loading in texture resources

#include "resource.h" //Contains the IDs of the game's resources

namespace Common
{
	namespace Sprites
	{
		//A list of all common sprites used for both the rooms and the branches

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
		//A list of all common textures used for both the rooms and the branches
		//These are then used by the Common Sprites

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

	//The main render window the game will take place in
	extern sf::RenderWindow MainWindow;

	//Creates all the common sprites used in the game
	void CreateSprites();

	//Gets a random sprite that can be used as a center tile
	sf::Sprite GetCenterSprite();

	//Gets a joint sprite that can be used as a joint piece pointing in a specified direction
	sf::Sprite GetJointPiece(Direction source, Direction to);
	//Gets a sprite that represents a side wall
	sf::Sprite GetSideSprite(Direction side);

	//Gets a sprite that represents a corner
	sf::Sprite GetCornerSprite(Direction A, Direction B);

	//Checks whether two sprites intersect. Optionally scaling their hitboxes by a scale factor
	bool SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, sf::Vector2f scaleFactor);
	//Checks whether two sprites intersect
	bool SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B);
	//Checks whether two sprites intersect. Optionally scaling their hitboxes by their texture sizes if set to true
	bool SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, bool scaleByTextureSize);

	//Refreshes the size of the window. This is normally used when the window gets resized
	void RefreshWindow(sf::RenderWindow& window = Common::MainWindow);
	//Gets the mouse position in world coordinates
	sf::Vector2f GetMouseWorldCoordinates(sf::RenderWindow& window = Common::MainWindow);

	//Centers the camera over a specified point
	void CenterCamera(sf::Vector2f center, sf::RenderWindow& window = Common::MainWindow);

	//Gets a random number between the minRange (inclusive) and the maxRange (exclusive)
	int RandomNumber(int minRange, int maxRange);
}