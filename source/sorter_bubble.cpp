#include <iostream>
#include <fstream>
#include <sstream>

#include "sorter_bubble.h"
#include "console.h"

using namespace luabridge;
using namespace std;
using namespace Console;

////////////////////////////////////////////////////////////////////////////////////////

string loadFileContents(const string &relativeFilePath)
{
    string sourceFilePath = __FILE__;
    string scriptDirectory = sourceFilePath.substr(0, sourceFilePath.find_last_of("/\\"));
    string filePath = scriptDirectory + "/" + relativeFilePath;

    ifstream file(filePath);
    if (!file.is_open())
    {
        return "";
    }

    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

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
    vector<int> orgArray = toSort;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    LuaRef tableToSort = newTable(L);

    for (size_t i = 0; i < orgArray.size(); ++i)
    {
        tableToSort[i + 1] = orgArray[i];
    }

    setGlobal(L, tableToSort, "cpp_toSort");

    string LUACode = loadFileContents("./bubblesort/bubblesort.lua");

    if (LUACode.empty())
    {
        throw runtime_error("Unable to load LUA code.");
    }

    int status = luaL_dostring(L, LUACode.c_str());

    if (status)
    {
        cout << "Error with lua code.\n" + string(lua_tostring(L, -1));
    }

    LuaRef luaSortedArray = getGlobal(L, "cpp_toSort");
    vector<int> sortedArray;
    size_t length = luaSortedArray.length();

    for (size_t i = 0; i < length; ++i)
    {
        LuaRef element = luaSortedArray[i + 1];
        if (element.isNumber())
        {
            sortedArray.push_back(element.cast<int>());
        }
    }

    lua_close(L);

    return sortedArray;
}