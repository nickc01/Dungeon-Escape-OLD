#include "Common.h" //Contains many common game functions and variables
#include <string> //Contains std::string
#include <time.h> //Contains the time() function for getting the current time
#include <random> //Contains the rand() and srand() functions for creating random values
#include "Math.h" //Contains many commonly used math functions
#include "Direction.h" //Contains the Direction Enum for specifying the direction

using namespace std; //Prevents me from having to type std everywhere
using namespace sf; //Prevents me from having to type sf everywhere

namespace
{
    //Create a sprite with the specified texture and origin point
    Sprite CreateSprite(const Texture& texture, Vector2f origin)
    {
        //Create the sprite with the set texture
        auto sprite = Sprite(texture);
        //Reset the sprite's position
        sprite.setPosition({0,0});
        //Set the sprite's origin
        sprite.setOrigin(origin);
        //Return the sprite
        return sprite;
    }

    //Create a sprite with the specified texture
    Sprite CreateSprite(const Texture& texture)
    {
        return CreateSprite(texture, static_cast<Vector2f>(texture.getSize() / 2u));
    }

    //Create a sprite with the specified texture and origin
    Sprite CreateSprite(const ResourceTexture& texture, Vector2f origin)
    {
        return CreateSprite(texture.GetTexture(),origin);
    }
    //Create a sprite with the specified texture
    Sprite CreateSprite(const ResourceTexture& texture)
    {
        return CreateSprite(texture.GetTexture());
    }
}

namespace Common
{
    namespace Sprites
    {
        //A list of all common sprites used for both the rooms and the branches

        Sprite centerPiece1;
        Sprite centerPiece2;

        Sprite topJointPiece;
        Sprite bottomJointPiece;
        Sprite leftJointPiece;
        Sprite rightJointPiece;

        Sprite topLeftPiece;
        Sprite topRightPiece;
        Sprite bottomLeftPiece;
        Sprite bottomRightPiece;
        Sprite topPiece;
        Sprite bottomPiece;
        Sprite rightPiece;
        Sprite leftPiece;
    }

    namespace Textures
    {
        //A list of all common textures used for both the rooms and the branches
        //These are then used by the Common Sprites

        ResourceTexture centerPiece1{CENTER_PIECE_1};
        ResourceTexture centerPiece2{CENTER_PIECE_2};

        ResourceTexture topJointPiece{TOP_JOINT};
        ResourceTexture bottomJointPiece{BOTTOM_JOINT};
        ResourceTexture leftJointPiece{LEFT_JOINT};
        ResourceTexture rightJointPiece{RIGHT_JOINT};

        ResourceTexture topLeftPiece{TOP_LEFT_PIECE};
        ResourceTexture topRightPiece{TOP_RIGHT_PIECE};
        ResourceTexture bottomLeftPiece{BOTTOM_LEFT_PIECE};
        ResourceTexture bottomRightPiece{BOTTOM_RIGHT_PIECE};
        ResourceTexture topPiece{TOP_PIECE};
        ResourceTexture bottomPiece{BOTTOM_PIECE};
        ResourceTexture rightPiece{RIGHT_PIECE};
        ResourceTexture leftPiece{LEFT_PIECE};
    }

    //The main render window the game will take place in
    RenderWindow MainWindow(VideoMode(800, 600), "Dungeon Escape");
}



using namespace Common::Textures; //Prevents me from having to type Common::Textures everywhere 

//Creates all the common sprites used in the game
void Common::CreateSprites()
{
    Sprites::centerPiece1 = CreateSprite(centerPiece1);
    Sprites::centerPiece2 = CreateSprite(centerPiece2);

    Sprites::topJointPiece = CreateSprite(topJointPiece);
    Sprites::bottomJointPiece = CreateSprite(bottomJointPiece);
    Sprites::leftJointPiece = CreateSprite(leftJointPiece);
    Sprites::rightJointPiece = CreateSprite(rightJointPiece);

    Sprites::topLeftPiece = CreateSprite(topLeftPiece);
    Sprites::topRightPiece = CreateSprite(topRightPiece);
    Sprites::bottomLeftPiece = CreateSprite(bottomLeftPiece);
    Sprites::bottomRightPiece = CreateSprite(bottomRightPiece);
    Sprites::topPiece = CreateSprite(topPiece);
    Sprites::bottomPiece = CreateSprite(bottomPiece);
    Sprites::rightPiece = CreateSprite(rightPiece);
    Sprites::leftPiece = CreateSprite(leftPiece);
}

//Gets a random sprite that can be used as a center tile
sf::Sprite Common::GetCenterSprite()
{
    //Pick a random number between 0 and 1
    int random = RandomNumber(0, 2);

    //The sprite we are going to select
    Sprite selectedSprite;

    //If the number is zero
    if (random == 0)
    {
        //Pick centerPiece1
        selectedSprite = Sprites::centerPiece1;
    }
    //If the number is one
    else
    {
        //Pick centerPiece2
        selectedSprite = Sprites::centerPiece2;
    }

    //Apply a random rotation to the sprite to vary it even more
    selectedSprite.setRotation(DirectionToDegrees(RandomDirection()));

    //Return the selected sprite
    return selectedSprite;
}

//Gets a joint sprite that can be used as a joint piece pointing in a specified direction
//SourceDirection : The direction where the joint is going
//To : The direction the joint is to redirect to

//IMPORTANT NOTE : In SFML, The negative y-axis points upwards, not downwards, because the y-axis starts at the top of the screen.
//                 This means that down points up, and up points down, so this will affect which sprite we choose
sf::Sprite Common::GetJointPiece(Direction sourceDirection, Direction to)
{
    //The selected joint sprite
    sf::Sprite joint = Common::Sprites::topJointPiece;

    switch (sourceDirection)
    {
        //If the source direction is going up
    case Direction::Up:
        //If the destination direction is to go left
        if (to == Direction::Left)
        {
            //Set the sprite to the left join piece
            joint = Common::Sprites::leftJointPiece;

            //Return the joint piece
            return joint;
        }
        //If the destination direction is to go right
        else if (to == Direction::Right)
        {
            //Set the sprite to the right join piece
            joint = Common::Sprites::rightJointPiece;

            //Return the joint piece
            return joint;
        }
        break;
        //If the source direction is going right
    case Direction::Right:
        //If the destination direction is to go down
        if (to == Direction::Down)
        {
            //Set the sprite to the top right joint piece
            joint = Common::Sprites::topJointPiece;

            //Flip the sprite horizontally so it's the top-left piece
            joint.scale({ -1.0f,1.0f });

            //Return the joint piece
            return joint;
        }
        //If the destination direction is to go up
        else if (to == Direction::Up)
        {
            //Set the sprite to the bottom right joint piece
            joint = Common::Sprites::bottomJointPiece;
            //Flip the sprite horizontally so it's the bottom-left piece
            joint.scale({ -1.0f,1.0f });

            //Return the joint piece
            return joint;
        }
        break;
        //If the source direction is going down
    case Direction::Down:
        //If the destination direction is to go left
        if (to == Direction::Left)
        {
            //Set the sprite to the bottom right joint piece
            joint = Common::Sprites::bottomJointPiece;
            //Flip the sprite horizontally so it's the bottom-left piece
            joint.scale({-1.0f,1.0f});

            //Return the joint piece
            return joint;
        }
        //If the destination direction is to go right
        else if (to == Direction::Right)
        {
            //Set the sprite to the bottom right joint piece
            joint = Common::Sprites::bottomJointPiece;

            //Return the joint piece
            return joint;
        }
        break;
        //If the source direction is going left
    case Direction::Left:
        //If the destination direction is to go down
        if (to == Direction::Down)
        {
            //Set the sprite to the top right joint piece
            joint = Common::Sprites::topJointPiece;

            //Return the joint piece
            return joint;
        }
        //If the destination direction is to go up
        else if (to == Direction::Up)
        {
            //Set the sprite to the bottom right joint piece
            joint = Common::Sprites::bottomJointPiece;

            //Return the joint piece
            return joint;
        }
        break;
    }
    return joint;
}

//Gets a sprite that represents a side wall
//IMPORTANT NOTE : In SFML, The negative y-axis points upwards, not downwards, because the y-axis starts at the top of the screen.
//                 This means that down points up, and up points down, so this will affect which sprite we choose
sf::Sprite Common::GetSideSprite(Direction side)
{
    switch (side)
    {
        //If we want the top wall side
    case Direction::Up:
        return Sprites::bottomPiece;

        //If we want the right wall side
    case Direction::Right:
        return Sprites::rightPiece;

        //If we want the bottom wall side
    case Direction::Down:
        return Sprites::topPiece;

        //If we want the left wall side
    case Direction::Left:
        return Sprites::leftPiece;
    }
    return Sprites::topPiece;
}

//Gets a sprite that represents a corner
//IMPORTANT NOTE : In SFML, The negative y-axis points upwards, not downwards, because the y-axis starts at the top of the screen.
//                 This means that down points up, and up points down, so this will affect which sprite we choose
sf::Sprite Common::GetCornerSprite(Direction A, Direction B)
{
    switch (A)
    {
        //If we are getting corner that is on the top side
    case Direction::Up:
        //If we are getting corner that is also on the left side
        if (B == Direction::Left)
        {
            //Return the bottom-left corner
            return Sprites::bottomLeftPiece;
        }
        //If we are getting corner that is also on the right side
        else if (B == Direction::Right)
        {
            //Return the bottom-right corner
            return Sprites::bottomRightPiece;
        }
        break;
        //If we are getting corner that is on the right side
    case Direction::Right:
        //If we are getting corner that is also on the top side
        if (B == Direction::Up)
        {
            //Return the bottom-right corner
            return Sprites::bottomRightPiece;
        }
        //If we are getting corner that is also on the bottom side
        else if (B == Direction::Down)
        {
            //Return the top-right corner
            return Sprites::topRightPiece;
        }
        break;
        //If we are getting corner that is on the bottom side
    case Direction::Down:
        //If we are getting corner that is also on the left side
        if (B == Direction::Left)
        {
            //Return the top-left corner
            return Sprites::topLeftPiece;
        }
        //If we are getting corner that is also on the right side
        else if (B == Direction::Right)
        {
            //Return the top-right corner
            return Sprites::topRightPiece;
        }
        break;
        //If we are getting corner that is on the left side
    case Direction::Left:
        //If we are getting corner that is also on the top side
        if (B == Direction::Up)
        {
            //Return the bottom-left corner
            return Sprites::bottomLeftPiece;
        }
        //If we are getting corner that is also on the bottom side
        else if (B == Direction::Down)
        {
            //Return the top-left corner
            return Sprites::topLeftPiece;
        }
        break;
    }
    //Return the top-left if no matches above were made
    return Sprites::topLeftPiece;
}

//Checks whether two sprites intersect. Optionally scaling their hitboxes by a scale factor
bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, Vector2f scaleFactor)
{
    //Get the global bounds of sprite A
    auto rectA = A.getGlobalBounds();
    //Get the global bounds of sprite B
    auto rectB = B.getGlobalBounds();

    //Scale their bounds by the specified scale factor
    rectA.left *= scaleFactor.x;
    rectA.top *= scaleFactor.y;
    rectB.top *= scaleFactor.y;
    rectB.left *= scaleFactor.x;

    //Check if they intersect
    return Math::RectsIntersect(rectA, rectB);
}

//Checks whether two sprites intersect
bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B)
{
    return Common::SpritesIntersect(A, B, { 1.0f,1.0f });
}

//Checks whether two sprites intersect. Optionally scaling their hitboxes by their texture sizes if set to true
bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, bool scaleByTextureSize)
{
    sf::Vector2f scaleFactor(1.0f, 1.0f);
    //If they are to be scaled based on their texture size
    if (scaleByTextureSize)
    {
        //Get the texture rect
        auto textureRect = A.getTextureRect();
        //Set the scale factor to the texture rect
        scaleFactor.x = textureRect.width;
        scaleFactor.y = textureRect.height;
    }

    return Common::SpritesIntersect(A, B, scaleFactor);
}

//Refreshes the size of the window. This is normally used when the window gets resized
void Common::RefreshWindow(sf::RenderWindow& window)
{
    //Get the current window view
    auto view = window.getView();
    //Reset the view's size
    view.setSize(static_cast<sf::Vector2f>(window.getSize() / 3u));
    //Set the window's view to the new one
    window.setView(view);
}

//Gets the mouse position in world coordinates
sf::Vector2f Common::GetMouseWorldCoordinates(sf::RenderWindow& window)
{
    //Gets the mouse's desktop position, subtracts the window position to get the window position of the mouse, and then maps that position to world coordinates
    return window.mapPixelToCoords(Mouse::getPosition() - window.getPosition());
}

//Centers the camera over a specified point
void Common::CenterCamera(Vector2f center, sf::RenderWindow& window)
{
    //Get the currently set view
    auto view = window.getView();

    //Update the window's view with the new view
    window.setView(View(center,view.getSize()));
}



bool randomRanOnce = false; //Whether the randomizer has been run at least once already

//Gets a random number between the minRange (inclusive) and the maxRange (exclusive)
int Common::RandomNumber(int minRange, int maxRange)
{
    //If it hasn't been ran already
    if (!randomRanOnce)
    {
        randomRanOnce = true;
        //Reset the starting seed
        srand(static_cast<unsigned int>(time(0)));
    }

    //Return a random number between the two ranges
    return (rand() % (maxRange - minRange)) + minRange;
}
