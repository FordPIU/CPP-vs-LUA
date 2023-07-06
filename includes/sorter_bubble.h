#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

class BubbleSort
{
public:
    BubbleSort(int param_sortSize)
    {
        std::vector<int> randomizedVector(param_sortSize);

        // Fill the vector with consecutive numbers
        for (int i = 0; i < param_sortSize; ++i)
        {
            randomizedVector[i] = i + 1;
        }

        // Randomly shuffle the vector
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(randomizedVector.begin(), randomizedVector.end(), g);

        toSort = randomizedVector;
    };

    std::vector<int> getToSort() { return toSort; }

    void CPP();
    void CPPMT();
    void LUA();
    void LUAMT();

private:
    std::vector<int> toSort;
};