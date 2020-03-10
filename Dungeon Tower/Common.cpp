#include "Common.h"
#include "Console.h"
#include <string>
#include <conio.h>
#include <time.h>
#include <random>

using namespace std;

namespace
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
}
