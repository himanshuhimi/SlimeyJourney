#pragma once

#include "core/config.h"

class Timer
{
public:
    SDL_Renderer *renderer = nullptr;
    Text text;
    double duration = 0.0, currentTime = 0.0;
    Timer(SDL_Renderer *renderer, double duration = 1.0);
    void handle(double dt);
    void render();
};