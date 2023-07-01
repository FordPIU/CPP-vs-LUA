#include "main.h"

#include <fstream>
#include <windows.h>
#include <psapi.h>
#include "TCHAR.h"
#include "pdh.h"

#include "sorter_bubble.h"
#include "timer.h"

// Note to self; re-organize this shitshow of a mess.

//////////////////////////////
// Because to make M/T CPP actually register times, LUA has to take a fucking lightyear and a half

string getHighestTimeUnit(int milliseconds)
{
    const char *units[] = {"ms", "sec", "min", "hr", "day"};
    int unitIndex = 0;

    while (milliseconds >= 1000 && unitIndex < 4)
    {
        milliseconds /= 1000;
        ++unitIndex;
    }

    return to_string(milliseconds) + units[unitIndex];
}

// Also here because *organization*
string getHighestMemoryUnit(long long memorySize)
{
    const char *units[] = {"bytes", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;

    while (memorySize >= 1024 && unitIndex < 4)
    {
        memorySize /= 1024;
        ++unitIndex;
    }

    return to_string(memorySize) + " " + units[unitIndex];
}

//////////////////////////////
// Resource Usage, a shit show of functions
// Off me please.

DWORDLONG getTotalVirtualMemory()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPageFile;
}

// Function to retrieve virtual memory currently used
DWORDLONG getVirtualMemoryUsed()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPageFile - memInfo.ullAvailPageFile;
}

// Function to retrieve virtual memory currently used by current process
SIZE_T getVirtualMemoryUsedByProcess()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
    return pmc.PrivateUsage;
}

// Function to retrieve total physical memory (RAM) size
DWORDLONG getTotalPhysicalMemory()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys;
}

// Function to retrieve physical memory currently used
DWORDLONG getPhysicalMemoryUsed()
{
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memInfo);
    return memInfo.ullTotalPhys - memInfo.ullAvailPhys;
}

// Function to retrieve physical memory currently used by current process
SIZE_T getPhysicalMemoryUsedByProcess()
{
    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS *)&pmc, sizeof(pmc));
    return pmc.WorkingSetSize;
}

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
            // Gets the optional Argument of sort size.
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

            // Init Bobble Sort with Sort Size
            BubbleSort bubblesort(sortSize);

            // Init Timer
            Timer timer;

            // Resource Usage
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