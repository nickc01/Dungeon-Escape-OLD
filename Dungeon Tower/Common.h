
#include <tuple>
#include <string>
#include <iostream>
#include <limits>

template<typename charType>
using MultiString = std::basic_string<charType, std::char_traits<charType>, std::allocator<charType>>;

template<typename charType>
using MultiStringStream = std::basic_stringstream<charType, std::char_traits<charType>, std::allocator<charType>>;



void WaitForInput();
void PrintTitleScreen();

void PrintInstructions();

int RandomNumber(int minRange, int maxRange);