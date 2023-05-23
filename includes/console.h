#pragma once

#include <iostream>
#include <string>
#include <windows.h>

namespace Console
{
    void print(std::string printString);
    void print(int printNum);
    void print(std::string printString, std::string printColor);

    std::string input(std::string header);
};