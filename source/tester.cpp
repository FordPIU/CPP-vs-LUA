#include <sstream>

#include "main.h"
#include "getResources.h"
#include "getHighestUnit.h"

#include "SortAlgorithm.h"
#include "Sort_BubbleSort.h"
#include "Sort_QuickSort.h"

#include "timer.h"

//////////////////////////////

/// @brief Prints out an arrays items
/// @param inout 0 = "In", while 1 = "Out".
/// @param array The array to print out
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

/// @brief Split the string by delimiter
/// @param input The String to Split
/// @param delimiter The Character to split the string by
/// @return vector of tokens, split by the delimiter
vector<string> splitString(string input, char delimiter)
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

/// @brief Request a input from the console
/// @param Title Header/Title of the Request
/// @return Splits the string by every space, so if the user inputs "hello world", it'll return {"hello", "world"}
vector<string> requestInput(string Title)
{
    string command = input(Title);
    return splitString(command, ' ');
}

/// @brief Takes a string with a math operator, and performs the math operation
/// @param expression Example: 500*2
/// @return int completed math operation
int evaluateExpression(string expression)
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
        default:
            throw runtime_error("Invalid operator");
        }

        iss >> op;
    }

    return result;
}

/// @brief Print out Memory Usage Information
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
        print("--------------------", "yellow");
        print("Tests:", "brightblue");
        print("bubblesort", "brightwhite");
        print("quicksort", "brightwhite");
        print("--------------------", "yellow");

        // Input and Arguments
        vector<string> args = requestInput("Execute Test");

        // Parse sort sizes and algorithms
        vector<int> sortSizes;
        vector<string> algorithms;

        // Process arguments
        string sortSizeArg = args[1];

        if (sortSizeArg.find_first_of('/') != string::npos)
        {
            vector<string> argSortSizes = splitString(sortSizeArg, '/');

            for (string size : argSortSizes)
            {
                if (size.find_first_of("*+-") != string::npos)
                {
                    sortSizes.push_back(evaluateExpression(size));
                }
                else
                {
                    sortSizes.push_back(stoi(size));
                }
            }
        }
        else
        {
            if (sortSizeArg.find_first_of("*+-") != string::npos)
            {
                sortSizes.push_back(evaluateExpression(sortSizeArg));
            }
            else
            {
                sortSizes.push_back(stoi(sortSizeArg));
            }
        }

        // Split algorithms by '/'
        string algoArg = args[0];
        if (algoArg.find_first_of('/') != string::npos)
        {
            vector<string> argAlgos = splitString(algoArg, '/');

            for (string algo : argAlgos)
            {
                algorithms.push_back(algo);
            }
        }
        else
        {
            algorithms.push_back(algoArg);
        }

        // Init Timer
        Timer timer;

        // Resource Usage
        poutResourceUsage();

        // Sort Algorithms
        vector<unique_ptr<SortAlgorithm>> sorters;

        for (string algorithm : algorithms)
        {
            if (algorithm == "bubblesort")
            {
                for (int size : sortSizes)
                {
                    sorters.push_back(make_unique<BubbleSort>(size));
                }
            }
            else if (algorithm == "quicksort")
            {
                for (int size : sortSizes)
                {
                    sorters.push_back(make_unique<QuickSort>(size));
                }
            }
            else
            {
                print("Invalid algorithm: " + algorithm, "red");
                continue;
            }
        }

        // Sort and measure time
        vector<int> cppTimes;
        vector<int> luaTimes;

        for (const auto &sorter : sorters)
        {
            print("\n");
            // CPP
            timer.startTimer();
            vector<int> sortedArrayCPP = sorter->CPP();
            int timeToCompleteCPP = timer.endTimer();
            cppTimes.push_back(timeToCompleteCPP);

            // LUA
            timer.startTimer();
            vector<int> sortedArrayLUA = sorter->LUA();
            int timeToCompleteLUA = timer.endTimer();
            luaTimes.push_back(timeToCompleteLUA);

            int sortSize = sorter->getToSortSize();
            string algorithm = sorter->getAlgo();

            print("     C++ " + algorithm + " took " + getHighestTimeUnit(timeToCompleteCPP) + "(" + to_string(timeToCompleteCPP) + ")ms" + " to sort " + to_string(sortSize) + ".", "green");
            print("     LUA " + algorithm + " took " + getHighestTimeUnit(timeToCompleteLUA) + "(" + to_string(timeToCompleteLUA) + ")ms" + " to sort " + to_string(sortSize) + ".", "green");
            // printArray(1, sortedArrayCPP);
            // printArray(1, sortedArrayLUA);
        }
    }
}
