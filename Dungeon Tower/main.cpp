#include <string>
#include <iostream>
#include "Renderer.h"
#include "Console.h"
#include "Common.h"
#include "Room.h"
#include "WorldMap.h"

using namespace std;


int main()
{
	Console::SetFullScreenConsole(true);

	PrintTitleScreen();

	PrintInstructions();

	Console::UseUnicode();

	//Console::SetFullScreenConsole(false);

	system("cls");

	WorldMap map;

	map.Generate(2);

	for (auto& room : map.GetFirstRoom()->GetRoomHierarchy())
	{
		room->Render();
	}

	//Vector2 windowSize = Console::GetConsoleWindowSize();

	//wcout << L"Window Size = " << get<0>(windowSize) << L", " << get<1>(windowSize) << L'\n';

	//Room room = Room({ 20,20 }, { 60,30 });

	//room.Render();

	WaitForInput();

	//Renderer::CameraPosition = windowSize / -2;

	/*Room room = Room({ get<0>(windowSize) / 2,get<1>(windowSize) / 2 }, { 60,30 });

	for (int x = 0; x < 60; x++)
	{
		for (int y = 0; y < 30; y++)
		{
			auto ptr = room[{x, y}];
			if (ptr != nullptr)
			{
				ptr->Render();
			}
		}
	}

	WaitForInput();*/

	//for (int i = 0; i <= 15; i++)
	//{
		//for (int j = 0; j <= 15; j++)
		//{
			//Console::DrawCharacter(i, i, L'&');
			//Console::SetColorAtPosition(i, i, static_cast<Color>(i), Black);
			//Console::SetColor(static_cast<Color>(i), static_cast<Color>(j));
			//wcout << L"&";
		//}
	//}

	/*int width = get<0>(Console::GetConsoleWindowSize());

	for (int i = 0; i <= 16 * 16; i++)
	{
		WORD Attributes;

		auto position = COORD();

		position.X = i % width;
		position.Y = i / width;

		DWORD charactersRead = 0;

		ReadConsoleOutputAttribute(Console::GetConsoleHandle(), &Attributes, 1, position, &charactersRead);

		Console::SetColor();

		wcout << L"Stats for character " << i << L'\n';

		wcout << boolalpha;

		wcout << L"Foreground Red = " << ((Attributes | FOREGROUND_RED) == FOREGROUND_RED) << "\n";
		wcout << L"Foreground Blue = " << ((Attributes | FOREGROUND_BLUE) == FOREGROUND_BLUE) << "\n";
		wcout << L"Foreground Green = " << ((Attributes | FOREGROUND_GREEN) == FOREGROUND_GREEN) << "\n";
		wcout << L"Foreground Intensity = " << ((Attributes | FOREGROUND_INTENSITY) == FOREGROUND_INTENSITY) << "\n";
		wcout << L"Background Red = " << ((Attributes | BACKGROUND_RED) == BACKGROUND_RED) << "\n";
		wcout << L"Background Blue = " << ((Attributes | BACKGROUND_BLUE) == BACKGROUND_BLUE) << "\n";
		wcout << L"Background Green = " << ((Attributes | BACKGROUND_GREEN) == BACKGROUND_GREEN) << "\n";
		wcout << L"Background Intensity = " << ((Attributes | BACKGROUND_INTENSITY) == BACKGROUND_INTENSITY) << "\n";
		wcout << "End\n\n";

		//Console::GetCharacter(Vector2(i, 0));
	}*/

	/*for (int i = 0; i < get<0>(Console::GetConsoleWindowSize()) - 1; i++)
	{
		Console::DrawCharacter(i, 10 + i, L'&');
	}*/

    //Console::PrintCentered(cout, "This should print centered"s);
	
}