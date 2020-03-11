#include "Console.h"
#include <io.h>
#include <fcntl.h>

using namespace std;

namespace 
{
	enum class AttributeColors {
		BLACK = 0,
		DARKBLUE = FOREGROUND_BLUE,
		DARKGREEN = FOREGROUND_GREEN,
		DARKCYAN = FOREGROUND_GREEN | FOREGROUND_BLUE,
		DARKRED = FOREGROUND_RED,
		DARKMAGENTA = FOREGROUND_RED | FOREGROUND_BLUE,
		DARKYELLOW = FOREGROUND_RED | FOREGROUND_GREEN,
		DARKGRAY = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		GRAY = FOREGROUND_INTENSITY,
		BLUE = FOREGROUND_INTENSITY | FOREGROUND_BLUE,
		GREEN = FOREGROUND_INTENSITY | FOREGROUND_GREEN,
		CYAN = FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_BLUE,
		RED = FOREGROUND_INTENSITY | FOREGROUND_RED,
		MAGENTA = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE,
		YELLOW = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN,
		WHITE = FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
	};

	enum class BackgroundAttributeColors {
		BLACK = 0,
		DARKBLUE = BACKGROUND_BLUE,
		DARKGREEN = BACKGROUND_GREEN,
		DARKCYAN = BACKGROUND_GREEN | BACKGROUND_BLUE,
		DARKRED = BACKGROUND_RED,
		DARKMAGENTA = BACKGROUND_RED | BACKGROUND_BLUE,
		DARKYELLOW = BACKGROUND_RED | BACKGROUND_GREEN,
		DARKGRAY = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
		GRAY = BACKGROUND_INTENSITY,
		BLUE = BACKGROUND_INTENSITY | BACKGROUND_BLUE,
		GREEN = BACKGROUND_INTENSITY | BACKGROUND_GREEN,
		CYAN = BACKGROUND_INTENSITY | BACKGROUND_GREEN | BACKGROUND_BLUE,
		RED = BACKGROUND_INTENSITY | BACKGROUND_RED,
		MAGENTA = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_BLUE,
		YELLOW = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN,
		WHITE = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE,
	};

	bool Contains(WORD Attributes, int macro)
	{
		return (Attributes | macro) == macro;
	}

	ColorPair TranslateAttributes(WORD Att)
	{
		Color foreground{};
		switch (static_cast<AttributeColors>(Att | (FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)))
		{
		case AttributeColors::BLACK:
			foreground = Black;
			break;
		case AttributeColors::DARKBLUE:
			foreground = Blue;
			break;
		case AttributeColors::DARKGREEN:
			foreground = Green;
			break;
		case AttributeColors::DARKCYAN:
			foreground = Aqua;
			break;
		case AttributeColors::DARKRED:
			foreground = Red;
			break;
		case AttributeColors::DARKMAGENTA:
			foreground = Purple;
			break;
		case AttributeColors::DARKYELLOW:
			foreground = Yellow;
			break;
		case AttributeColors::DARKGRAY:
			foreground = Gray;
			break;
		case AttributeColors::GRAY:
			foreground = White;
			break;
		case AttributeColors::BLUE:
			foreground = LightBlue;
			break;
		case AttributeColors::GREEN:
			foreground = LightGreen;
			break;
		case AttributeColors::CYAN:
			foreground = LightAqua;
			break;
		case AttributeColors::RED:
			foreground = LightRed;
			break;
		case AttributeColors::MAGENTA:
			foreground = LightPurple;
			break;
		case AttributeColors::YELLOW:
			foreground = LightYellow;
			break;
		case AttributeColors::WHITE:
			foreground = BrightWhite;
			break;
		}
		Color background{};
		switch (static_cast<BackgroundAttributeColors>(Att | (BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE)))
		{
		case BackgroundAttributeColors::BLACK:
			background = Black;
			break;
		case BackgroundAttributeColors::DARKBLUE:
			background = Blue;
			break;
		case BackgroundAttributeColors::DARKGREEN:
			background = Green;
			break;
		case BackgroundAttributeColors::DARKCYAN:
			background = Aqua;
			break;
		case BackgroundAttributeColors::DARKRED:
			background = Red;
			break;
		case BackgroundAttributeColors::DARKMAGENTA:
			background = Purple;
			break;
		case BackgroundAttributeColors::DARKYELLOW:
			background = Yellow;
			break;
		case BackgroundAttributeColors::DARKGRAY:
			background = Gray;
			break;
		case BackgroundAttributeColors::GRAY:
			background = White;
			break;
		case BackgroundAttributeColors::BLUE:
			background = LightBlue;
			break;
		case BackgroundAttributeColors::GREEN:
			background = LightGreen;
			break;
		case BackgroundAttributeColors::CYAN:
			background = LightAqua;
			break;
		case BackgroundAttributeColors::RED:
			background = LightRed;
			break;
		case BackgroundAttributeColors::MAGENTA:
			background = LightPurple;
			break;
		case BackgroundAttributeColors::YELLOW:
			background = LightYellow;
			break;
		case BackgroundAttributeColors::WHITE:
			background = BrightWhite;
			break;
		}
		return ColorPair(foreground, background);
	}

	WORD TranslateColors(Color foreground, Color background)
	{
		AttributeColors foregroundAttributes;

		switch (foreground)
		{
		case Black:
			foregroundAttributes = AttributeColors::BLACK;
			break;
		case Blue:
			foregroundAttributes = AttributeColors::DARKBLUE;
			break;
		case Green:
			foregroundAttributes = AttributeColors::DARKGREEN;
			break;
		case Aqua:
			foregroundAttributes = AttributeColors::DARKCYAN;
			break;
		case Red:
			foregroundAttributes = AttributeColors::DARKRED;
			break;
		case Purple:
			foregroundAttributes = AttributeColors::DARKMAGENTA;
			break;
		case Yellow:
			foregroundAttributes = AttributeColors::DARKYELLOW;
			break;
		case White:
			foregroundAttributes = AttributeColors::GRAY;
			break;
		case Gray:
			foregroundAttributes = AttributeColors::DARKGRAY;
			break;
		case LightBlue:
			foregroundAttributes = AttributeColors::BLUE;
			break;
		case LightGreen:
			foregroundAttributes = AttributeColors::GREEN;
			break;
		case LightAqua:
			foregroundAttributes = AttributeColors::CYAN;
			break;
		case LightRed:
			foregroundAttributes = AttributeColors::RED;
			break;
		case LightPurple:
			foregroundAttributes = AttributeColors::MAGENTA;
			break;
		case LightYellow:
			foregroundAttributes = AttributeColors::YELLOW;
			break;
		case BrightWhite:
			foregroundAttributes = AttributeColors::WHITE;
			break;
		}

		BackgroundAttributeColors backgroundAttributes;

		switch (background)
		{
		case Black:
			backgroundAttributes = BackgroundAttributeColors::BLACK;
			break;
		case Blue:
			backgroundAttributes = BackgroundAttributeColors::DARKBLUE;
			break;
		case Green:
			backgroundAttributes = BackgroundAttributeColors::DARKGREEN;
			break;
		case Aqua:
			backgroundAttributes = BackgroundAttributeColors::DARKCYAN;
			break;
		case Red:
			backgroundAttributes = BackgroundAttributeColors::DARKRED;
			break;
		case Purple:
			backgroundAttributes = BackgroundAttributeColors::DARKMAGENTA;
			break;
		case Yellow:
			backgroundAttributes = BackgroundAttributeColors::DARKYELLOW;
			break;
		case White:
			backgroundAttributes = BackgroundAttributeColors::GRAY;
			break;
		case Gray:
			backgroundAttributes = BackgroundAttributeColors::DARKGRAY;
			break;
		case LightBlue:
			backgroundAttributes = BackgroundAttributeColors::BLUE;
			break;
		case LightGreen:
			backgroundAttributes = BackgroundAttributeColors::GREEN;
			break;
		case LightAqua:
			backgroundAttributes = BackgroundAttributeColors::CYAN;
			break;
		case LightRed:
			backgroundAttributes = BackgroundAttributeColors::RED;
			break;
		case LightPurple:
			backgroundAttributes = BackgroundAttributeColors::MAGENTA;
			break;
		case LightYellow:
			backgroundAttributes = BackgroundAttributeColors::YELLOW;
			break;
		case BrightWhite:
			backgroundAttributes = BackgroundAttributeColors::WHITE;
			break;
		}

		return static_cast<WORD>(foregroundAttributes) | static_cast<WORD>(backgroundAttributes);
	}
}

HANDLE Console::GetConsoleHandle()
{
	return GetStdHandle(STD_OUTPUT_HANDLE);
}

HANDLE Console::GetConsoleInputHandle()
{
	return GetStdHandle(STD_INPUT_HANDLE);
}

void Console::SetFullScreenConsole(bool fullscreen)
{
	DWORD flags = 0;
	if (fullscreen)
	{
		flags = CONSOLE_FULLSCREEN_MODE;
	}
	else
	{
		flags = CONSOLE_WINDOWED_MODE;
	}

	SetConsoleDisplayMode(GetConsoleHandle(), flags, nullptr);
}

//Gets an input record from the console. Returns null if no inputs are buffered
std::unique_ptr<INPUT_RECORD> Console::GetConsoleInput()
{
	INPUT_RECORD record;
	DWORD recordsRead;
	if (PeekConsoleInputA(GetConsoleInputHandle(), &record, sizeof(INPUT_RECORD), &recordsRead))
	{
		ReadConsoleInputA(GetConsoleInputHandle(), &record, sizeof(INPUT_RECORD), &recordsRead);
		return std::move(make_unique<INPUT_RECORD>(record));
	}
	else
	{
		return std::move(unique_ptr<INPUT_RECORD>(nullptr));
	}
}

Vector2 Console::GetCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFOEX info{};
	GetConsoleScreenBufferInfoEx(GetConsoleHandle(), &info);
	return Vector2(info.dwCursorPosition.X, info.dwCursorPosition.Y);
}

void Console::SetCursorPosition(Vector2 position)
{
	CONSOLE_SCREEN_BUFFER_INFOEX info{};
	GetConsoleScreenBufferInfoEx(GetConsoleHandle(), &info);

	info.dwCursorPosition.X = get<0>(position);
	info.dwCursorPosition.Y = get<1>(position);

	SetConsoleScreenBufferInfoEx(GetConsoleHandle(), &info);
}

Vector2 Console::GetConsoleWindowSize()
{
	CONSOLE_SCREEN_BUFFER_INFO info{};
	GetConsoleScreenBufferInfo(GetConsoleHandle(), &info);
	return Vector2(info.srWindow.Right - info.srWindow.Left + 1, info.srWindow.Bottom - info.srWindow.Top + 1);
}

void Console::SetCursorVisibility(bool visible)
{
	CONSOLE_CURSOR_INFO info;
	GetConsoleCursorInfo(GetConsoleHandle(), &info);

	info.bVisible = visible;

	SetConsoleCursorInfo(GetConsoleHandle(), &info);
}

void Console::UseUnicode()
{
	_setmode(_fileno(stdout), _O_WTEXT);
}

void Console::UseAscii()
{
	_setmode(_fileno(stdout), _O_TEXT);
}

void Console::ClearInputBuffer()
{
	FlushConsoleInputBuffer(GetConsoleInputHandle());
}

void Console::DrawCharacter(Vector2 position, wchar_t character)
{
	DrawCharacter(get<0>(position), get<1>(position), character);
}

void Console::DrawCharacter(int x, int y, wchar_t character)
{
	CHAR_INFO info{};

	info.Char.UnicodeChar = character;

	info.Attributes = 0;

	auto position = COORD();

	position.X = x;
	position.Y = y;

	DWORD writtenChars = 0;

	FillConsoleOutputCharacterW(GetConsoleHandle(), character, 1, position, &writtenChars);
}

wchar_t Console::GetCharacter(Vector2 position)
{
	return Console::GetCharacter(get<0>(position), get<1>(position));
}

wchar_t Console::GetCharacter(int x, int y)
{
	wchar_t character;

	auto pos = COORD();

	pos.X = x;
	pos.Y = y;

	DWORD charsRead = 0;

	ReadConsoleOutputCharacterW(GetConsoleHandle(), &character, 1,pos,&charsRead);

	return character;
}

ColorPair Console::GetColorAtPosition(Vector2 position)
{
	return GetColorAtPosition(get<0>(position), get<1>(position));
}

ColorPair Console::GetColorAtPosition(int x, int y)
{
	WORD Attributes;

	auto position = COORD();

	position.X = x;
	position.Y = y;

	DWORD charactersRead = 0;

	ReadConsoleOutputAttribute(GetConsoleHandle(), &Attributes, 1, position, &charactersRead);

	return TranslateAttributes(Attributes);
}

void Console::SetColorAtPosition(Vector2 position, ColorPair colors)
{
	SetColorAtPosition(get<0>(position), get<1>(position), colors);
}

void Console::SetColorAtPosition(int x, int y, ColorPair colors)
{
	SetColorAtPosition(x, y, get<0>(colors), get<1>(colors));
}

void Console::SetColorAtPosition(Vector2 position, Color foreground, Color background)
{
	SetColorAtPosition(get<0>(position), get<1>(position), foreground, background);
}

void Console::SetColorAtPosition(int x, int y, Color foreground, Color background)
{
	auto pos = COORD();

	pos.X = x;
	pos.Y = y;

	DWORD charactersWritten;

	FillConsoleOutputAttribute(GetConsoleHandle(), TranslateColors(foreground, background), 1, pos, &charactersWritten);
}

void Console::ClearScreen()
{
	auto dimensions = GetConsoleWindowSize();
	DWORD CharsWritten;
	FillConsoleOutputCharacterW(GetConsoleHandle(), L' ', get<0>(dimensions) * get<1>(dimensions), { 0,0 }, &CharsWritten);
	FillConsoleOutputAttribute(GetConsoleHandle(),0, get<0>(dimensions) * get<1>(dimensions), { 0,0 }, &CharsWritten);
}

void Console::SetColor(Color Text, Color Background)
{
	//Retrieve the handle to the console window
	HANDLE consoleHandle = GetConsoleHandle();

	//Set the background and text color the console
	SetConsoleTextAttribute(consoleHandle, Text + (Background * 16));
}
