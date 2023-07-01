#pragma once

#ifndef GET_HIGHEST_UNIT_H
#define GET_HIGHEST_UNIT_H

#include <string>

using namespace std;

#endif // GET_HIGHEST_UNIT_H

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