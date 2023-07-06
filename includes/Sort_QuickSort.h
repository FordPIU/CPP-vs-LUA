#pragma once

#include "SortAlgorithm.h"

class QuickSort : public SortAlgorithm
{
public:
    QuickSort(int param_sortSize) : SortAlgorithm(param_sortSize) {}
    vector<int> CPP() override
    {
        vector<int> arrayToSort = getToSort();
        quickSort(arrayToSort, 0, arrayToSort.size() - 1);
        return arrayToSort;
    }

    vector<int> LUA() override
    {
        return lua::ExecLua(getToSort(), "../source/LUA/quicksort.lua");
    }

private:
    void quickSort(vector<int> &array, int low, int high)
    {
        if (low < high)
        {
            int pivotIndex = partition(array, low, high);
            quickSort(array, low, pivotIndex - 1);
            quickSort(array, pivotIndex + 1, high);
        }
    }

    int partition(vector<int> &array, int low, int high)
    {
        int pivot = array[high];
        int i = low - 1;

        for (int j = low; j < high; ++j)
        {
            if (array[j] < pivot)
            {
                ++i;
                swap(array[i], array[j]);
            }
        }

        swap(array[i + 1], array[high]);
        return i + 1;
    }
};