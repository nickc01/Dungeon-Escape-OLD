#include "Common.h"
#include <string>
#include <conio.h>
#include <time.h>
#include <random>
#include "Direction.h"

using namespace std;
using namespace sf;

namespace
{
    Sprite CreateSprite(const Texture& texture, Vector2f origin)
    {
        auto sprite = Sprite(texture);
        sprite.setPosition({0,0});
        sprite.setOrigin(origin);
        return sprite;
    }

    Sprite CreateSprite(const Texture& texture)
    {
        return CreateSprite(texture, static_cast<Vector2f>(texture.getSize() / 2u));
    }

    Sprite CreateSprite(const ResourceTexture& texture, Vector2f origin)
    {
        return CreateSprite(texture.GetTexture(),origin);
    }

    Sprite CreateSprite(const ResourceTexture& texture)
    {
        return CreateSprite(texture.GetTexture());
    }
}

namespace Common
{
    namespace Sprites
    {
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
        ResourceTexture centerPiece1{CENTER_PIECE_1};
        ResourceTexture centerPiece2{CENTER_PIECE_2};

        //ResourceTexture jointPiece{JOINTPIECE};
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

    RenderWindow MainWindow(VideoMode(800, 600), "This is a test");
}



using namespace Common::Textures;


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

sf::Sprite Common::GetCenterSprite()
{
    int random = RandomNumber(0, 2);

    Sprite centerSprite = Sprites::centerPiece1;

    if (random == 0)
    {
        centerSprite = Sprites::centerPiece2;
    }

    centerSprite.setRotation(DirectionToDegrees(RandomDirection()));

    return centerSprite;
}

//SourceDirection : The direction where the joint is going
//To : The direction the joint is to redirect to
sf::Sprite Common::GetJointPiece(Direction sourceDirection, Direction to)
{
    sf::Sprite joint = Common::Sprites::topJointPiece;

    switch (sourceDirection)
    {
    case Direction::Up:
        if (to == Direction::Left)
        {
            joint = Common::Sprites::leftJointPiece;
            return joint;
        }
        else if (to == Direction::Right)
        {
            joint = Common::Sprites::rightJointPiece;
            return joint;
        }
        break;
    case Direction::Right:
        if (to == Direction::Down)
        {
            joint = Common::Sprites::topJointPiece;
            joint.scale({ -1.0f,1.0f });
            return joint;
        }
        else if (to == Direction::Up)
        {
            joint = Common::Sprites::bottomJointPiece;
            joint.scale({ -1.0f,1.0f });
            return joint;
        }
        break;
    case Direction::Down:
        if (to == Direction::Left)
        {
            joint = Common::Sprites::bottomJointPiece;
            joint.scale({-1.0f,1.0f});
            return joint;
        }
        else if (to == Direction::Right)
        {
            joint = Common::Sprites::bottomJointPiece;
            //joint.scale({ 1.0f,-1.0f });
            return joint;
        }
        break;
    case Direction::Left:
        if (to == Direction::Down)
        {
            joint = Common::Sprites::topJointPiece;
            return joint;
        }
        else if (to == Direction::Up)
        {
            joint = Common::Sprites::bottomJointPiece;
            return joint;
        }
        break;
    }
    return joint;
}

sf::Sprite Common::GetSideSprite(Direction side)
{
    switch (side)
    {
    case Direction::Up:
        return Sprites::bottomPiece;
    case Direction::Right:
        return Sprites::rightPiece;
    case Direction::Down:
        return Sprites::topPiece;
    case Direction::Left:
        return Sprites::leftPiece;
    }
    return Sprites::topPiece;
}

sf::Sprite Common::GetCornerSprite(Direction A, Direction B)
{
    switch (A)
    {
    case Direction::Up:
        if (B == Direction::Left)
        {
            return Sprites::bottomLeftPiece;
        }
        else if (B == Direction::Right)
        {
            return Sprites::bottomRightPiece;
        }
        break;
    case Direction::Right:
        if (B == Direction::Up)
        {
            return Sprites::bottomRightPiece;
        }
        else if (B == Direction::Down)
        {
            return Sprites::topRightPiece;
        }
        break;
    case Direction::Down:
        if (B == Direction::Left)
        {
            return Sprites::topLeftPiece;
        }
        else if (B == Direction::Right)
        {
            return Sprites::topRightPiece;
        }
        break;
    case Direction::Left:
        if (B == Direction::Up)
        {
            return Sprites::bottomLeftPiece;
        }
        else if (B == Direction::Down)
        {
            return Sprites::topLeftPiece;
        }
        break;
    }
    return Sprites::topLeftPiece;
}

bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, Vector2f scaleFactor)
{
    auto rectA = A.getGlobalBounds();
    auto rectB = B.getGlobalBounds();

    rectA.left *= scaleFactor.x;
    rectA.top *= scaleFactor.y;
    rectB.top *= scaleFactor.y;
    rectB.left *= scaleFactor.x;

    auto leftA = rectA.left;
    auto rightA = rectA.left + rectA.width;
    auto topA = rectA.top;
    auto bottomA = rectA.top - rectA.height;

    auto leftB = rectB.left;
    auto rightB = rectB.left + rectB.width;
    auto topB = rectB.top;
    auto bottomB = rectB.top - rectB.height;

    if (leftA < rightB && rightA > leftB&& topA > bottomB&& bottomA < topB)
    {

        return true;
    }
    return false;
}

bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B)
{
    return Common::SpritesIntersect(A, B, { 1.0f,1.0f });
}

bool Common::SpritesIntersect(const sf::Sprite& A, const sf::Sprite& B, bool scaleByTextureSize)
{
    sf::Vector2f scaleFactor(1.0f, 1.0f);
    if (scaleByTextureSize)
    {
        auto textureRect = A.getTextureRect();
        scaleFactor.x = textureRect.width;
        scaleFactor.y = textureRect.height;
    }

    return Common::SpritesIntersect(A, B, scaleFactor);
}

void RefreshWindow(sf::RenderWindow& window)
{
    auto view = window.getView();
    view.setSize(static_cast<sf::Vector2f>(window.getSize() / 3u));
    window.setView(view);
}

sf::Vector2f GetMouseWorldCoordinates(sf::RenderWindow& window)
{
    return window.mapPixelToCoords(Mouse::getPosition() - window.getPosition());
}

void CenterCamera(Vector2f center, sf::RenderWindow& window)
{
    auto view = window.getView();
    //view.setCenter(center);
    window.setView(View(center,view.getSize()));
}



bool randomRanOnce = false;

int RandomNumber(int minRange, int maxRange)
{
    if (!randomRanOnce)
    {
        randomRanOnce = true;
        srand(static_cast<unsigned int>(time(0)));
    }

    return (rand() % (maxRange - minRange)) + minRange;
}
