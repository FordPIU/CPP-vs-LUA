#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

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

    std::vector<int> CPP();
    std::vector<int> LUA();

private:
    std::vector<int> toSort;
};