#include <cctype>
#include <algorithm>
#include "console.h"

using namespace std;

bool hasAnythingPrinted = false;

void setTextColor(int colorCode)
{
    HANDLE col;
    col = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, colorCode);
}

string toLowercase(const string &input)
{
    string result = input;
    transform(result.begin(), result.end(), result.begin(),
              [](unsigned char c)
              { return tolower(c); });
    return result;
}

int getTextColorFromString(string plainColor)
{
    string lwrPlainColor = toLowercase(plainColor);
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

    if (lwrPlainColor == "red")
    {
        return 4;
    }
    if (lwrPlainColor == "blue")
    {
        return 1;
    }
    if (lwrPlainColor == "brightblue")
    {
        return 9;
    }
    if (lwrPlainColor == "green")
    {
        return 2;
    }
    if (lwrPlainColor == "yellow")
    {
        return 14;
    }
    if (lwrPlainColor == "brightwhite")
    {
        return 15;
    }
    if (lwrPlainColor == "grey")
    {
        return 8;
    }

    return 7;
}

///////////

// String Print
void Console::print(string printString)
{
    if (hasAnythingPrinted)
    {
        cout << '\n'
             << printString;
    }
    else
    {
        cout << printString;
        hasAnythingPrinted = true;
    }
}

// Integer Print
void Console::print(int printNum)
{
    if (hasAnythingPrinted)
    {
        cout << '\n'
             << to_string(printNum);
    }
    else
    {
        cout << to_string(printNum);
        hasAnythingPrinted = true;
    }
}

// String Colored Print
void Console::print(string printString, string printColor)
{
    // Coloring
    setTextColor(getTextColorFromString(printColor));

    // Printing
    if (hasAnythingPrinted)
    {
        cout << '\n'
             << printString;
    }
    else
    {
        cout << printString;
        hasAnythingPrinted = true;
    }
}

// String Input
string Console::input(string header)
{
    string output = "";

    Console::print(header + ": ", "red");
    getline(cin, output);

    return output;
}