#include "main.h"

#include "sorter_bubble.h"
#include "timer.h"

vector<string> splitString(const string &input, char delimiter)
{
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while (getline(ss, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

int main()
{
    while (true)
    {
        // Tests Print
        print("--------------------", "yellow");
        print("Tests:", "brightblue");
        print("bubblesort", "brightwhite");
        print("--------------------", "yellow");

        // Input and Arguments
        string command = input("Execute Test");
        vector<string> arguments = splitString(command, ' ');

        // Master Handler
        if (arguments[0] == "bubblesort")
        {
            int sortSize = 100;

            if (arguments.size() > 1)
            {
                string arg = arguments[1];
                if (arg.find("*") != string::npos)
                {
                    vector<string> hs = splitString(arg, '*');
                    int lhsVal = stoi(hs[0]);
                    int rhsVal = stoi(hs[1]);
                    sortSize = lhsVal * rhsVal;
                }
                else
                {
                    sortSize = stoi(arg);
                }
            }

            // Bubble Sort
            BubbleSort bubblesort(sortSize);

            // Timer
            Timer timer;

            // CPP
            timer.startTimer();
            bubblesort.CPP();

            int timeToComplete = timer.endTimer();
            print("C++ Bubblesort took " + to_string(timeToComplete) + "ms to sort " + to_string(sortSize) + ".");

            // LUA
            lua_State *L = luaL_newstate();
            luaL_openlibs(L);
            luabridge::LuaRef toSortTable = luabridge::newTable(L);
            vector<int> toSort = bubblesort.getToSort();

            for (size_t i = 0; i < toSort.size(); ++i)
            {
                toSortTable[i + 1] = toSort[i];
            }

            timer.startTimer();
            bubblesort.LUA(L, toSortTable);

            int LUAtimeToComplete = timer.endTimer();
            print("LUA Bubblesort took " + to_string(LUAtimeToComplete) + "ms to sort " + to_string(sortSize) + ".");
        }
    }
}