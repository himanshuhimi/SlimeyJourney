#pragma once

#include "config.h"
#include "tools/level.h"
#include "ui/button.h"
#include "tools/ui.h"

class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    bool active = false;
    double dt = 0.0;
    States state = States::HOME;
    Game();
    void launch();
    void render();
    void handle();
    void update(States newState, bool loading = true);
    void nextLevel();
    void terminate();

private:
    Level *currentLevel = nullptr;
    UI<Game *> *ui = nullptr;
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
    void updateStateTexts();
};