#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <algorithm>

#include "algo_lua.h"

using namespace std;

class SortAlgorithm
{
public:
    SortAlgorithm(int param_sortSize)
    {
        vector<int> randomizedVector(param_sortSize);

        for (int i = 0; i < param_sortSize; ++i)
        {
            randomizedVector[i] = i + 1;
        }

        random_device rd;
        mt19937 g(rd());
        shuffle(randomizedVector.begin(), randomizedVector.end(), g);

        toSort = randomizedVector;
    }

    vector<int> getToSort() { return toSort; }
    int getToSortSize() { return toSort.size(); }

    virtual string getAlgo() = 0;

    virtual vector<int> CPP() = 0;
    virtual vector<int> LUA() = 0;

private:
    vector<int> toSort;
};