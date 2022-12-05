#include "Clock.hpp"


Clock::Clock() {
    this->currentFrame = SDL_GetPerformanceCounter();
    this->lastFrame = 0;
    this->CalcDeltaTime();
}

float Clock::DeltaTime() const {
    return this->deltaTime;
}

void Clock::Update() {
    this->lastFrame = this->currentFrame;
    this->currentFrame = SDL_GetPerformanceCounter();
    this->CalcDeltaTime();
}