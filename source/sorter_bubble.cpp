#include <iostream>
#include <thread>
#include "sorter_bubble.h"

using namespace luabridge;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

void bubbleSort(vector<int> &arr, int start, int end)
{
    for (int i = start; i < end - 1; ++i)
    {
        for (int j = start; j < end - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1])
            {
                swap(arr[j], arr[j + 1]);
            }
        }
    }
}

void merge(vector<int> &arr, int start, int mid, int end)
{
    vector<int> temp(end - start);
    int i = start, j = mid, k = 0;

    while (i < mid && j < end)
    {
        if (arr[i] <= arr[j])
            temp[k++] = arr[i++];
        else
            temp[k++] = arr[j++];
    }

    while (i < mid)
        temp[k++] = arr[i++];

    while (j < end)
        temp[k++] = arr[j++];

    for (int p = 0; p < k; ++p)
        arr[start + p] = temp[p];
}

////////////////////////////////////////////////////////////////////////////////////////

void BubbleSort::CPP()
{
    vector<int> sortedArray = toSort;
    int n = sortedArray.size();
    bool swapped;

    for (int i = 0; i < n - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (sortedArray[j] > sortedArray[j + 1])
            {
                swap(sortedArray[j], sortedArray[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

void BubbleSort::CPPMT()
{
    vector<int> sortedArray = toSort;
    int n = sortedArray.size();
    int numThreads = thread::hardware_concurrency(); // Get the number of available threads

    vector<thread> threads;
    int chunkSize = n / numThreads; // Divide the array into equal chunks

    // Create threads and assign sorting tasks
    for (int i = 0; i < numThreads - 1; ++i)
    {
        threads.emplace_back(bubbleSort, ref(sortedArray), i * chunkSize, (i + 1) * chunkSize);
    }
    // Handle the remaining portion of the array
    threads.emplace_back(bubbleSort, ref(sortedArray), (numThreads - 1) * chunkSize, n);

    // Wait for all threads to finish
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Merge the sorted chunks
    for (int i = 0; i < numThreads - 1; ++i)
    {
        merge(sortedArray, i * chunkSize, (i + 1) * chunkSize, (i + 2) * chunkSize);
    }

    // Handle the remaining portion of the array
    merge(sortedArray, (numThreads - 1) * chunkSize, n - chunkSize, n);

    // sortedArray now contains the fully sorted array
}

void BubbleSort::LUA(lua_State *L, luabridge::LuaRef toSortTable)
{
    luabridge::setGlobal(L, toSortTable, "cpp_toSort");

    string LUACode = R"(
    local sortedArray = {}
    local n = #cpp_toSort
    local swapped

    for i = 1, n - 1 do
        swapped = false
        for j = 1, n - i do
            if cpp_toSort[j] > cpp_toSort[j + 1] then
                local temp = cpp_toSort[j]
                cpp_toSort[j] = cpp_toSort[j + 1]
                cpp_toSort[j + 1] = temp
                swapped = true
            end
        end

        if not swapped then
            break
        end
    end
    )";

    int status = luaL_dostring(L, LUACode.c_str());
    if (status)
    {
        cout << "Error with lua code.\n" + string(lua_tostring(L, -1));
    }

    lua_close(L);
}