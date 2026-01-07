#include "engine/Time.h"

double Time::lastTime = 0.0;
float Time::dt = 0.0f;
float Time::speed = 1.0f;

float Time::control(double currentTime) {
    return static_cast<float>(currentTime * speed);
}

void Time::update(double currentTime) {
    dt = static_cast<float>(currentTime - lastTime);
    lastTime = currentTime;
}

float Time::deltaTime() {
    return dt;
}