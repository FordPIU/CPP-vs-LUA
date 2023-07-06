#include <sstream>

#include "main.h"
#include "getResources.h"
#include "getHighestUnit.h"

#include "sorter_bubble.h"
#include "timer.h"

//////////////////////////////

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

vector<string> requestInput(string Title)
{
    string command = input(Title);
    return splitString(command, ' ');
}

int evaluateExpression(const string &expression)
{
    istringstream iss(expression);
    int result = 0;
    char op = '+';
    int operand;

    while (iss >> operand)
    {
        switch (op)
        {
        case '+':
            result += operand;
            break;
        case '-':
            result -= operand;
            break;
        case '*':
            result *= operand;
            break;
        case '/':
            if (operand != 0)
                result /= operand;
            else
                throw runtime_error("Division by zero");
            break;
        default:
            throw runtime_error("Invalid operator");
        }

        iss >> op;
    }

    return result;
}

void poutResourceUsage()
{
    // Total Virtual Memory
    DWORDLONG totalVirtualMemory = getTotalVirtualMemory();
    string totalVirtualMemoryStr = getHighestMemoryUnit(totalVirtualMemory);
    print("Total Virtual Memory: " + totalVirtualMemoryStr, "grey");

    // Virtual Memory currently used
    DWORDLONG virtualMemoryUsed = getVirtualMemoryUsed();
    string virtualMemoryUsedStr = getHighestMemoryUnit(virtualMemoryUsed);
    print("Virtual Memory currently used: " + virtualMemoryUsedStr, "grey");

    // Virtual Memory currently used by current process
    SIZE_T virtualMemoryUsedByProcess = getVirtualMemoryUsedByProcess();
    string virtualMemoryUsedByProcessStr = getHighestMemoryUnit(virtualMemoryUsedByProcess);
    print("Virtual Memory currently used by current process: " + virtualMemoryUsedByProcessStr, "yellow");

    // Total Physical Memory (RAM)
    DWORDLONG totalPhysicalMemory = getTotalPhysicalMemory();
    string totalPhysicalMemoryStr = getHighestMemoryUnit(totalPhysicalMemory);
    print("Total Physical Memory: " + totalPhysicalMemoryStr, "grey");

    // Physical Memory currently used
    DWORDLONG physicalMemoryUsed = getPhysicalMemoryUsed();
    string physicalMemoryUsedStr = getHighestMemoryUnit(physicalMemoryUsed);
    print("Physical Memory currently used: " + physicalMemoryUsedStr, "grey");

    // Physical Memory currently used by current process
    SIZE_T physicalMemoryUsedByProcess = getPhysicalMemoryUsedByProcess();
    string physicalMemoryUsedByProcessStr = getHighestMemoryUnit(physicalMemoryUsedByProcess);
    print("Physical Memory currently used by current process: " + physicalMemoryUsedByProcessStr, "yellow");
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
        vector<string> args = requestInput("Execute Test");

        // Gets the optional Argument of sort size.
        int sortSize = 100;

        if (args.size() > 1)
        {
            string arg = args[1];
            size_t operatorPos = arg.find_first_of("*/+-");

            if (operatorPos != string::npos)
            {
                sortSize = evaluateExpression(arg);
            }
            else
            {
                sortSize = stoi(arg);
            }
        }

        // Master Handler
        if (args[0] == "bubblesort")
        {
            // Init Bobble Sort with Sort Size
            BubbleSort bubblesort(sortSize);

            // Init Timer
            Timer timer;

            // Resource Usage
            poutResourceUsage();

            // CPP Bubblesort
            {
                timer.startTimer();
                bubblesort.CPP();

                int timeToComplete = timer.endTimer();
                print("     C++ Bubblesort took " + getHighestTimeUnit(timeToComplete) + " to sort " + to_string(sortSize) + ".", "green");
            }

            // CPP Multi-thread Bubblesort
            {
                timer.startTimer();
                bubblesort.CPPMT();

                int timeToComplete = timer.endTimer();
                print("     C++ M/T Bubblesort took " + getHighestTimeUnit(timeToComplete) + " to sort " + to_string(sortSize) + ".", "green");
            }

            // LUA Bubblesort
            {
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
                print("     LUA Bubblesort took " + getHighestTimeUnit(LUAtimeToComplete) + " to sort " + to_string(sortSize) + ".", "green");
            }
        }
    }
}