#pragma once

class Time 
{
public:
    static void update(double currentTime);
    static float deltaTime();
    static float simDeltaTime();
    static void setSimSpeed(float newSpeed);
    static float getSimSpeed();
    static void togglePause();
    static bool isPaused();
private:
    static double lastTime;
    static float realDT;
    static float simDT;
    static float simSpeed;
    static bool paused;
};