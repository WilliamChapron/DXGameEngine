#pragma once
#include <Windows.h>

class Time {
private:
    LARGE_INTEGER startTime;
    LARGE_INTEGER frequency;

    LARGE_INTEGER currentTime;
    LARGE_INTEGER lastTime;
public:

    Time();

    void UpdateTime();

    double GetElapsedTime();
    double GetDeltaTime();
    int GetFramePerSecond();


};