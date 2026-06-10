#pragma once

#include "config.h"
#include "tools/level.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active = false;
    double dt = 0.0;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();

private:
    Level *currentLevel = nullptr;
    vector<Level *> levels = {};
    vector<Text> texts = {};
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    States nextState;
    int levelNum = 0;
    bool uiInit = false;
    void updateDeltaTime();
    void loadLevels();
    void updateLevel();
    void manageUpdation();
};