#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Vector2.h"
#include "Common.h"
#include <memory>
#include <string>
#include <iostream>
#include <vector>

enum Color
{
	Black = 0,
	Blue = 1,
	Green = 2,
	Aqua = 3,
	Red = 4,
	Purple = 5,
	Yellow = 6,
	White = 7,
	Gray = 8,
	LightBlue = 9,
	LightGreen = 10,
	LightAqua = 11,
	LightRed = 12,
	LightPurple = 13,
	LightYellow = 14,
	BrightWhite = 15
};

using ColorPair = std::tuple<Color, Color>;

namespace Console
{
	HANDLE GetConsoleHandle();

	HANDLE GetConsoleInputHandle();

	void SetFullScreenConsole(bool fullscreen);

	std::unique_ptr<INPUT_RECORD> GetConsoleInput();

	Vector2 GetCursorPosition();

	void SetCursorPosition(Vector2 position);

	Vector2 GetConsoleWindowSize();

	void SetCursorVisibility(bool visible);

	void UseUnicode();

	void UseAscii();

	void ClearInputBuffer();

	void DrawCharacter(Vector2 position,wchar_t character);
	void DrawCharacter(int x, int y, wchar_t character);

	wchar_t GetCharacter(Vector2 position);
	wchar_t GetCharacter(int x, int y);

	ColorPair GetColorAtPosition(Vector2 position);
	ColorPair GetColorAtPosition(int x, int y);

	void SetColorAtPosition(Vector2 position, ColorPair colors);
	void SetColorAtPosition(Vector2 position, Color foreground, Color background);
	void SetColorAtPosition(int x, int y, ColorPair colors);
	void SetColorAtPosition(int x, int y, Color foreground, Color background);

	template<typename charType>
	const std::vector<MultiString<charType>> SplitLines(MultiString<charType> input,const charType delimiter = static_cast<charType>('\n'))
	{
		std::vector<MultiString<charType>> LineList{};

		MultiString<charType> currentLine;

		for (const charType& character : input)
		{
			if (character == delimiter)
			{
				LineList.push_back(currentLine);

				currentLine.clear();
			}
			else
			{
				currentLine += character;
			}
		}

		if (currentLine.size() > 0)
		{
			LineList.push_back(currentLine);
		}

		return std::move(LineList);
	}

	template<typename streamType,typename charType>
	void PrintCentered(streamType& stream, MultiString<charType> string, charType fillerLeft = static_cast<charType>(' '), charType fillerRight = static_cast<charType>(' '))
	{
		int consoleWidth = std::get<0>(Console::GetConsoleWindowSize());

		auto lines = SplitLines(string);

		for (const MultiString<charType>& line : lines)
		{
			int lineWidth = line.size();
			if (lineWidth >= consoleWidth)
			{
				stream << line;
			}
			else
			{
				int extraSpace = consoleWidth - lineWidth;
				size_t extraSpaceLeft = extraSpace / 2;
				size_t extraSpaceRight = extraSpace - extraSpaceLeft;

				stream << MultiString<charType>(extraSpaceLeft, fillerLeft) << line << MultiString<charType>(extraSpaceRight, fillerRight);
			}
		}
	}

	template<typename streamType, typename charType>
	void PrintRight(streamType& stream, MultiString<charType> string, charType fillerLeft = static_cast<charType>(' '))
	{
		int consoleWidth = std::get<0>(Console::GetConsoleWindowSize());

		auto lines = SplitLines(string);

		for (const MultiString<charType>& line : lines)
		{
			int lineWidth = line.size();
			if (lineWidth >= consoleWidth)
			{
				stream << line;
			}
			else
			{
				size_t extraSpaceLeft = consoleWidth - lineWidth;

				stream << MultiString<charType>(extraSpaceLeft, fillerLeft) << line;
			}
		}

	}

	void SetColor(Color Text = BrightWhite, Color Background = Black);
}
