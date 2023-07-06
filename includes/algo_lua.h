#pragma once

#ifndef ALGO_LUA_H
#define ALGO_LUA_H
#include <vector>
#include <fstream>
#include <sstream>
#include <lua.hpp>
#include <LuaBridge/LuaBridge.h>

using namespace std;
using namespace luabridge;
#endif // ALGO_LUA_H

namespace lua
{
    namespace
    {
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
    }
    vector<int> ExecLua(vector<int> toSort, string filePath)
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

        string LUACode = loadFileContents(filePath);

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
}