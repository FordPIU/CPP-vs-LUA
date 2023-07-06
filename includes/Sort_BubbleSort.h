#pragma once

#include "SortAlgorithm.h"

class BubbleSort : public SortAlgorithm
{
public:
    BubbleSort(int param_sortSize) : SortAlgorithm(param_sortSize) {}

    string getAlgo() override
    {
        return "BubbleSort";
    }

    vector<int> CPP() override
    {
        vector<int> orgArray = getToSort();
        int arrayLength = static_cast<int>(orgArray.size());
        bool swapped;

        for (int i = 0; i < arrayLength - 1; ++i)
        {
            swapped = false;
            for (int j = 0; j < arrayLength - i - 1; ++j)
            {
                if (orgArray[j] > orgArray[j + 1])
                {
                    swap(orgArray[j], orgArray[j + 1]);
                    swapped = true;
                }
            }

            if (!swapped)
                break;
        }

        return orgArray;
    }

    vector<int> LUA() override
    {
        return lua::ExecLua(getToSort(), "../source/LUA/bubblesort.lua");
    }
};