#include "Common.h"
#include <string>
#include <conio.h>
#include <time.h>
#include <random>

using namespace std;
using namespace sf;

namespace Common
{
    namespace Sprites
    {
        Sprite blankTile;

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
        ResourceTexture blankTexture;
        ResourceTexture cornerPiece;
        ResourceTexture sidePiece;
    }

    RenderWindow window(VideoMode(800, 600), "This is a test");
}



using namespace Common::Textures;
using namespace Common::Sprites;


void Common::CreateSprites()
{
    blankTexture = ResourceTexture(BLANK_TILE);
    cornerPiece = ResourceTexture(TOP_LEFT);
    sidePiece = ResourceTexture(TOP_PIECE);

    auto textureSize = blankTexture.GetTexture().getSize();


    blankTile = Sprite(blankTexture.GetTexture());
    blankTile.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

    topLeftPiece = Sprite(cornerPiece.GetTexture());
    topPiece = Sprite(sidePiece.GetTexture());

    topLeftPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    topPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);

    topRightPiece = Sprite(cornerPiece.GetTexture());
    rightPiece = Sprite(sidePiece.GetTexture());

    topRightPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    rightPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    topRightPiece.rotate(90);
    rightPiece.rotate(90);

    bottomRightPiece = Sprite(cornerPiece.GetTexture());
    bottomPiece = Sprite(sidePiece.GetTexture());
    bottomRightPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    bottomPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    bottomRightPiece.rotate(180);
    bottomPiece.rotate(180);

    bottomLeftPiece = Sprite(cornerPiece.GetTexture());
    leftPiece = Sprite(sidePiece.GetTexture());
    bottomLeftPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    leftPiece.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    bottomLeftPiece.rotate(270);
    leftPiece.rotate(270);
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
