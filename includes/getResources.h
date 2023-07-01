#pragma once

#include <fstream>
#include <windows.h>
#include <psapi.h>
#include "TCHAR.h"
#include "pdh.h"

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