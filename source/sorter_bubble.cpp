#include <iostream>
#include <thread>
#include <mutex>

#include "sorter_bubble.h"
#include "console.h"

using namespace luabridge;
using namespace std;
using namespace Console;

////////////////////////////////////////////////////////////////////////////////////////

vector<int> CPPBubbleSort(vector<int> arrayToSort)
{
    int arrayLength = arrayToSort.size();
    bool swapped;

    for (int i = 0; i < arrayLength - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < arrayLength - i - 1; ++j)
        {
            if (arrayToSort[j] > arrayToSort[j + 1])
            {
                swap(arrayToSort[j], arrayToSort[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return arrayToSort;
}

vector<int> BubbleSort::CPP()
{
    return CPPBubbleSort(toSort);
}

vector<int> BubbleSort::CPPMT()
{
    int arrayLength = toSort.size();
    int numThreads = thread::hardware_concurrency();

    if (numThreads > arrayLength)
    {
        numThreads = arrayLength;
    }

    vector<thread> threads;
    mutex chunksMutex;
    int chunkSize = arrayLength / numThreads;

    vector<vector<int>> sortedChunks(numThreads);

    for (int i = 0; i < numThreads; ++i)
    {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? arrayLength : start + chunkSize;

        vector<int> chunkArray(toSort.begin() + start, toSort.begin() + end);

        threads.emplace_back([&chunkArray, &sortedChunks, &chunksMutex, i]()
                             {
            CPPBubbleSort(chunkArray);

            lock_guard<mutex> lock(chunksMutex);

            sortedChunks[i] = chunkArray; });
    }

    for (auto &thread : threads)
    {
        thread.join();
    }

    vector<int> mergedArray;
    for (const auto &chunk : sortedChunks)
    {
        mergedArray.insert(mergedArray.end(), chunk.begin(), chunk.end());
    }

    return CPPBubbleSort(mergedArray);
}

////////////////////////////////////////////////////////////////////////////////////////

vector<int> LUABubbleSort(vector<int> arrayToSort)
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    LuaRef tableToSort = newTable(L);

    for (size_t i = 0; i < arrayToSort.size(); ++i)
    {
        tableToSort[i + 1] = arrayToSort[i];
    }

    setGlobal(L, tableToSort, "cpp_toSort");

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

    LuaRef luaSortedArray = getGlobal(L, "cpp_toSort");
    vector<int> sortedArray;

    if (lua_istable(L, -1))
    {
        size_t length = luaSortedArray.length();
        for (size_t i = 0; i < length; ++i)
        {
            LuaRef element = luaSortedArray[i + 1];
            if (element.isNumber())
            {
                sortedArray.push_back(element.cast<int>());
            }
        }
    }

    lua_close(L);

    return sortedArray;
}

vector<int> BubbleSort::LUA()
{
    return LUABubbleSort(toSort);
}

vector<int> BubbleSort::LUAMT()
{
    return toSort;
}