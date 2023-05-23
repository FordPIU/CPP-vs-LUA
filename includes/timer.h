#include <windows.h>
#include <iostream>

class Timer
{
public:
    void startTimer()
    {
        if (timerStart != 0)
        {
            throw std::runtime_error("ATTEMPT TO START TIMER WHILE TIMER RUNNING\n");
        }

        timerStart = getTickCount();
    }
    int endTimer()
    {
        int returnTime = getTickDifferent(timerStart, getTickCount());

        timerStart = 0;

        return returnTime;
    }

private:
    int timerStart = 0;

    int getTickCount() { return static_cast<int>(GetTickCount64()); }
    int getTickDifferent(int tick1, int tick2) { return tick2 - tick1; }
};