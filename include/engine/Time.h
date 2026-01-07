#pragma once

class Time 
{
public:
    static float control(double currentTime);
    static void update(double currentTime);
    static float deltaTime();

private:
    static double lastTime;
    static float dt;
    static float speed;
};