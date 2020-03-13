#include "Common.h"
#include "Console.h"
#include <string>
#include <conio.h>
#include <time.h>
#include <random>

using namespace std;



sf::Sprite Common::Sprites::blankTile;

sf::Sprite Common::Sprites::topLeftPiece;
sf::Sprite Common::Sprites::topRightPiece;
sf::Sprite Common::Sprites::bottomLeftPiece;
sf::Sprite Common::Sprites::bottomRightPiece;
sf::Sprite Common::Sprites::topPiece;
sf::Sprite Common::Sprites::bottomPiece;
sf::Sprite Common::Sprites::rightPiece;
sf::Sprite Common::Sprites::leftPiece;





ResourceTexture Common::Textures::blankTexture;
ResourceTexture Common::Textures::cornerPiece;
ResourceTexture Common::Textures::sidePiece;

sf::RenderWindow Common::window(sf::VideoMode(800, 600), "This is a test");

/*namespace
{

    bool randomRanOnce = false;


    const auto Title = LR"(


▓█████▄  █    ██  ███▄    █   ▄████ ▓█████  ▒█████   ███▄    █    ▄▄▄█████▓ ▒█████   █     █░▓█████  ██▀███  
▒██▀ ██▌ ██  ▓██▒ ██ ▀█   █  ██▒ ▀█▒▓█   ▀ ▒██▒  ██▒ ██ ▀█   █    ▓  ██▒ ▓▒▒██▒  ██▒▓█░ █ ░█░▓█   ▀ ▓██ ▒ ██▒
░██   █▌▓██  ▒██░▓██  ▀█ ██▒▒██░▄▄▄░▒███   ▒██░  ██▒▓██  ▀█ ██▒   ▒ ▓██░ ▒░▒██░  ██▒▒█░ █ ░█ ▒███   ▓██ ░▄█ ▒
░▓█▄   ▌▓▓█  ░██░▓██▒  ▐▌██▒░▓█  ██▓▒▓█  ▄ ▒██   ██░▓██▒  ▐▌██▒   ░ ▓██▓ ░ ▒██   ██░░█░ █ ░█ ▒▓█  ▄ ▒██▀▀█▄  
░▒████▓ ▒▒█████▓ ▒██░   ▓██░░▒▓███▀▒░▒████▒░ ████▓▒░▒██░   ▓██░     ▒██▒ ░ ░ ████▓▒░░░██▒██▓ ░▒████▒░██▓ ▒██▒
 ▒▒▓  ▒ ░▒▓▒ ▒ ▒ ░ ▒░   ▒ ▒  ░▒   ▒ ░░ ▒░ ░░ ▒░▒░▒░ ░ ▒░   ▒ ▒      ▒ ░░   ░ ▒░▒░▒░ ░ ▓░▒ ▒  ░░ ▒░ ░░ ▒▓ ░▒▓░
 ░ ▒  ▒ ░░▒░ ░ ░ ░ ░░   ░ ▒░  ░   ░  ░ ░  ░  ░ ▒ ▒░ ░ ░░   ░ ▒░       ░      ░ ▒ ▒░   ▒ ░ ░   ░ ░  ░  ░▒ ░ ▒░
 ░ ░  ░  ░░░ ░ ░    ░   ░ ░ ░ ░   ░    ░   ░ ░ ░ ▒     ░   ░ ░      ░      ░ ░ ░ ▒    ░   ░     ░     ░░   ░ 
   ░       ░              ░       ░    ░  ░    ░ ░           ░                 ░ ░      ░       ░  ░   ░     
 ░                                                                                                           


)"s;

}

void WaitForInput()
{
    _getch();
    Console::ClearInputBuffer();
}

void PrintTitleScreen()
{
    system("cls");

    Console::SetCursorVisibility(false);

    Console::UseUnicode();

	Console::SetColor(Red);

    Console::PrintCentered(wcout, Title);

    Console::UseAscii();

    Console::SetColor(BrightWhite);

    Console::PrintCentered(cout, "-----Press Any Key to Begin-----"s);

    WaitForInput();
}

void PrintInstructions()
{
    system("cls");

    cout << string(10, '\n');

    Console::PrintCentered(cout, "-----Instructions-----"s);

    cout << "\n\n\n";

    Console::SetColor(Green);

    Console::PrintCentered(cout, "This is you -> I\n\n"s);

    Console::SetColor(BrightWhite);

    Console::PrintCentered(cout, "Use the arrow keys or WASD to move your character around the map\n\n"s);

    Console::SetColor(LightYellow);

    Console::PrintCentered(cout, "You have a bow and arrow\n"s);
    Console::PrintCentered(cout, "Use your mouse to aim and left click to fire\n\n"s);

    Console::SetColor(Red);

    Console::PrintCentered(cout, "Avoid These -> # @ &\n\n"s);

    Console::SetColor(Green);

    Console::PrintCentered(cout, "Find the door to escape -> D\n\n"s);

    Console::SetColor(BrightWhite);

    Console::PrintCentered(cout, "Don't Die\n\n"s);

    Console::SetColor(BrightWhite);

    Console::PrintCentered(cout, "Press any key to begin the game"s);

    WaitForInput();
}

int RandomNumber(int minRange, int maxRange)
{
    if (!randomRanOnce)
    {
        randomRanOnce = true;
        srand(time(0));
    }

    return (rand() % (maxRange - minRange)) + minRange;
}*/

void Common::CreateSprites()
{
    Textures::blankTexture = ResourceTexture(BLANK_TILE);
    Textures::cornerPiece = ResourceTexture(TOP_LEFT);
    Textures::sidePiece = ResourceTexture(TOP_PIECE);

    auto textureSize = Textures::blankTexture.GetTexture().getSize();


    Sprites::blankTile = sf::Sprite(Textures::blankTexture.GetTexture());
    Sprites::blankTile.setOrigin(textureSize.x / 2, textureSize.y / 2);

    Sprites::topLeftPiece = sf::Sprite(Textures::cornerPiece.GetTexture());
    Sprites::topPiece = sf::Sprite(Textures::sidePiece.GetTexture());

    Sprites::topLeftPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::topPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);

    Sprites::topRightPiece = sf::Sprite(Textures::cornerPiece.GetTexture());
    Sprites::rightPiece = sf::Sprite(Textures::sidePiece.GetTexture());

    Sprites::topRightPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::rightPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::topRightPiece.rotate(90);
    Sprites::rightPiece.rotate(90);

    Sprites::bottomRightPiece = sf::Sprite(Textures::cornerPiece.GetTexture());
    Sprites::bottomPiece = sf::Sprite(Textures::sidePiece.GetTexture());
    Sprites::bottomRightPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::bottomPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::bottomRightPiece.rotate(180);
    Sprites::bottomPiece.rotate(180);

    Sprites::bottomLeftPiece = sf::Sprite(Textures::cornerPiece.GetTexture());
    Sprites::leftPiece = sf::Sprite(Textures::sidePiece.GetTexture());
    Sprites::bottomLeftPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::leftPiece.setOrigin(textureSize.x / 2, textureSize.y / 2);
    Sprites::bottomLeftPiece.rotate(270);
    Sprites::leftPiece.rotate(270);
}



bool randomRanOnce = false;

int RandomNumber(int minRange, int maxRange)
{
    if (!randomRanOnce)
    {
        randomRanOnce = true;
        srand(time(0));
    }

    return (rand() % (maxRange - minRange)) + minRange;
}
