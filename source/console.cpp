#include "console.h"

bool hasAnythingPrinted = false;

void setTextColor(int colorCode)
{
    HANDLE col;
    col = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, colorCode);
}

int getTextColorFromString(std::string plainColor)
{
    /*
    1  : Blue
    2  : Green
    3  : Cyan
    4  : Red
    5  : Purple
    6  : Yellow(dark)
    7  : Default white
    8  : Grey
    9  : Bright blue
    10 : Bright green
    11 : Bright cyan
    12 : Bright red
    13 : pink
    14 : yellow
    15 : Bright white
    */

    if (plainColor == "red")
    {
        return 4;
    }
    if (plainColor == "blue")
    {
        return 1;
    }
    if (plainColor == "brightblue")
    {
        return 9;
    }
    if (plainColor == "green")
    {
        return 2;
    }
    if (plainColor == "yellow")
    {
        return 14;
    }
    if (plainColor == "brightwhite")
    {
        return 15;
    }

    return 7;
}

///////////

// String Print
void Console::print(std::string printString)
{
    if (hasAnythingPrinted)
    {
        std::cout << '\n'
                  << printString;
    }
    else
    {
        std::cout << printString;
        hasAnythingPrinted = true;
    }
}

// Integer Print
void Console::print(int printNum)
{
    if (hasAnythingPrinted)
    {
        std::cout << '\n'
                  << std::to_string(printNum);
    }
    else
    {
        std::cout << std::to_string(printNum);
        hasAnythingPrinted = true;
    }
}

// String Colored Print
void Console::print(std::string printString, std::string printColor)
{
    // Coloring
    setTextColor(getTextColorFromString(printColor));

    // Printing
    if (hasAnythingPrinted)
    {
        std::cout << '\n'
                  << printString;
    }
    else
    {
        std::cout << printString;
        hasAnythingPrinted = true;
    }
}

// String Input
std::string Console::input(std::string header)
{
    std::string output = "";

    Console::print(header + ": ", "red");
    getline(std::cin, output);

    return output;
}