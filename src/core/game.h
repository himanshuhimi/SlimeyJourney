#pragma once

#include "config.h"
#include "tools/level.h"
#include "tools/ui.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    States nextState;
    Level *currentLevel = nullptr;
    bool active = false;
    double dt = 0.0;
    Game();
    void launch();
    void render();
    void handle();
    void terminate();

private:
    vector<Level *> levels = {};
    vector<Text> texts = {};
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    UI<Game *> * ui = nullptr;
    int levelNum = 0;
    void updateDeltaTime();
    void loadLevels();
    void updateLevel();
    void manageUpdation();
    void collision();
};