#include <iostream>

#include "sorter_bubble.h"
#include "algo_lua.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

vector<int> BubbleSort::CPP()
{
    vector<int> orgArray = toSort;
    int arrayLength = orgArray.size();
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

////////////////////////////////////////////////////////////////////////////////////////

vector<int> BubbleSort::LUA()
{
    return lua::ExecLua(toSort, "../source/bubblesort/bubblesort.lua");
}