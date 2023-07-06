#include <iostream>
#include <thread>

#include "sorter_bubble.h"
#include "console.h"

using namespace luabridge;
using namespace std;
using namespace Console;

////////////////////////////////////////////////////////////////////////////////////////

vector<int> BubbleSort::CPP()
{
    int arrayLength = toSort.size();
    bool swapped;

    for (int i = 0; i < arrayLength - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < arrayLength - i - 1; ++j)
        {
            if (toSort[j] > toSort[j + 1])
            {
                swap(toSort[j], toSort[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }

    return toSort;
}

////////////////////////////////////////////////////////////////////////////////////////

vector<int> BubbleSort::LUA()
{
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    LuaRef tableToSort = newTable(L);

    for (size_t i = 0; i < toSort.size(); ++i)
    {
        tableToSort[i + 1] = toSort[i];
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