#pragma once

#include "config.h"
#include "../tools/level.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active = false;
    double dt;
    std::unordered_map<string, Audio *> audios;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();
private:
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    Level *level;
    void updateDeltaTime();
};