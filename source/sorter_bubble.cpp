#include <iostream>
#include <thread>
#include <optional>
#include "sorter_bubble.h"

using namespace luabridge;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////

optional<vector<int>> CPPBubbleSort(vector<int> arrayToSort, bool wantReturn = false)
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

    if (wantReturn)
    {
        return arrayToSort;
    }
}

void BubbleSort::CPP()
{
    CPPBubbleSort(toSort);
}

void BubbleSort::CPPMT()
{
    int arrayLength = toSort.size();
    int numThreads = thread::hardware_concurrency();

    vector<thread> threads;
    int chunkSize = arrayLength / numThreads;

    for (int i = 0; i < numThreads - 1; ++i)
    {
        vector<int> chunkArray(toSort.begin() + (i * chunkSize), toSort.begin() + ((i + 1) * chunkSize));
        threads.emplace_back([&chunkArray]()
                             { CPPBubbleSort(chunkArray, true); });
    }

    vector<int> lastChunkArray(toSort.begin() + ((numThreads - 1) * chunkSize), toSort.begin() + arrayLength);
    threads.emplace_back([&lastChunkArray]()
                         { CPPBubbleSort(lastChunkArray, true); });

    for (auto &thread : threads)
    {
        thread.join();
    }
}

////////////////////////////////////////////////////////////////////////////////////////

void LUABubbleSort(vector<int> arrayToSort)
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

    lua_close(L);
}

void BubbleSort::LUA()
{
    LUABubbleSort(toSort);
}

void BubbleSort::LUAMT()
{
}