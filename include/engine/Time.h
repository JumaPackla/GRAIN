#pragma once

class Time 
{
public:
    static void update(double currentTime);
    static float deltaTime();

private:
    static double lastTime;
    static float dt;
};