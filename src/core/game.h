#pragma once

#include "config.h"
#include "tools/systems/settings.h"
#include "tools/systems/level.h"
#include "tools/systems/region.h"
#include "tools/systems/ui.h"

class UI;
class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    Settings *settings = nullptr;
    Scenes scene, nextScene;
    Level *crntLvl = nullptr;
    Region *crntRgn = nullptr;
    bool active = false;
    double dt = 0.0;
    Game();
    void launch();
    void render();
    void handle();
    void setScene(Scenes newState, bool loading = true);
    void terminate();
    void loadLevels();

private:
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    using Levels = map<int, Level *>;
    map<string, Levels> regions = {};
    map<string, vector<int>> lvlNums = {};
    map<string, int> rgnMaxLvls = {};
    UI *ui = nullptr;
    string crntRgnName = "tutorial";
    int lvlNum = 0;
    void updateDeltaTime();
    void updateLevel();
    void collision();
};