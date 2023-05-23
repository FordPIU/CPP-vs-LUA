#include <iostream>
#include "sorter_bubble.h"

using namespace luabridge;

void BubbleSort::CPP()
{
    std::vector<int> sortedArray = toSort;
    int n = sortedArray.size();
    bool swapped;

    for (int i = 0; i < n - 1; ++i)
    {
        swapped = false;
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (sortedArray[j] > sortedArray[j + 1])
            {
                std::swap(sortedArray[j], sortedArray[j + 1]);
                swapped = true;
            }
        }

        if (!swapped)
            break;
    }
}

void BubbleSort::LUA(lua_State *L, luabridge::LuaRef toSortTable)
{
    luabridge::setGlobal(L, toSortTable, "cpp_toSort");

    std::string LUACode = R"(
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
        std::cout << "Error with lua code.\n" + std::string(lua_tostring(L, -1));
    }

    lua_close(L);
}