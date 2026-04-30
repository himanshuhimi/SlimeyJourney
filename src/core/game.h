#pragma once

#include "config.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active = false;
    double dt;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();

private:
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    void updateDeltaTime();
};