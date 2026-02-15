#include "engine/Time.h"
#include <iostream>

double Time::lastTime = 0.0;
float Time::realDT = 0.0f;
float Time::simDT = 0.0f;
float Time::simSpeed = 1.0f;
bool Time::paused = false;

void Time::update(double currentTime)
{
    realDT = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;

    simDT = realDT * simSpeed * !paused;
}

float Time::deltaTime()
{
    return realDT;
}

float Time::simDeltaTime()
{
    return simDT;
}

void Time::setSimSpeed(float newSpeed)
{
    simSpeed = newSpeed;
}

float Time::getSimSpeed()
{
    return simSpeed;
}

void Time::togglePause()
{
    paused = !paused;
}

bool Time::isPaused()
{
    return paused;
}