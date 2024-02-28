#include "Time.h"
#include <math.h>

Time::Time()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&startTime);
    lastTime = startTime;
    currentTime = startTime;
}

void Time::UpdateTime()
{
    lastTime = currentTime;

    QueryPerformanceCounter(&currentTime);
}

double Time::GetElapsedTime() {
    return static_cast<double>(currentTime.QuadPart - startTime.QuadPart) / frequency.QuadPart;
}

double Time::GetDeltaTime(){
    double deltaTime = static_cast<double>(currentTime.QuadPart - lastTime.QuadPart) / frequency.QuadPart;

    return deltaTime;
}

int Time::GetFramePerSecond()
{
    return static_cast<int>(floor(1.0f / GetDeltaTime()));
}


