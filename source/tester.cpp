#include <sstream>

#include "main.h"
#include "getResources.h"
#include "getHighestUnit.h"

#include "SortAlgorithm.h"
#include "Sort_BubbleSort.h"
#include "Sort_QuickSort.h"

#include "timer.h"

//////////////////////////////

void printArray(int inout, vector<int> array)
{
    HANDLE col;
    col = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(col, 11);

    cout << endl
         << ((inout == 0) ? "In: " : "Out: ");
    for (auto &number : array)
    {
        cout << to_string(number) + ", ";
    }
    cout << endl;
}

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
        print("quicksort", "brightwhite");
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

        auto sorter = unique_ptr<SortAlgorithm>();

        if (args[0] == "bubblesort")
        {
            sorter = make_unique<BubbleSort>(sortSize);
        }
        else if (args[0] == "quicksort")
        {
            sorter = make_unique<QuickSort>(sortSize);
        }

        // Init Timer
        Timer timer;

        // Resource Usage
        poutResourceUsage();

        // Print out initial array
        // printArray(0, bubblesort.getToSort());

        // CPP
        {
            timer.startTimer();

            vector<int> sortedArray = sorter->CPP();
            int timeToComplete = timer.endTimer();

            print("     C++ " + args[0] + " took " + getHighestTimeUnit(timeToComplete) + " to sort " + to_string(sortSize) + ".", "green");
            // printArray(1, sortedArray);
        }

        // LUA
        {
            timer.startTimer();

            vector<int> sortedArray = sorter->LUA();
            int LUAtimeToComplete = timer.endTimer();

            print("     LUA " + args[0] + " took " + getHighestTimeUnit(LUAtimeToComplete) + " to sort " + to_string(sortSize) + ".", "green");
            // printArray(1, sortedArray);
        }
    }
}