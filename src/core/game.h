#pragma once

#include "config.h"
#include "tools/systems/settings.h"
#include "tools/systems/level.h"
#include "tools/systems/region.h"
#include "entities/passives/cloud.h"
#include "entities/passives/petal.h"
#include "tools/systems/ui.h"
#include "tools/parallax.h"

class UI;
class Game
{
public:
    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    Settings *settings = nullptr;
    Scenes scene = Scenes::HOME, prevScene = Scenes::HOME, nextScene = Scenes::HOME;
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
    void unloadLevels();
    void setLevel(string region, int number);

private:
    vector<Cloud> clouds = {};
    vector<Petal> petals = {};
    Parallax *parallax;
    UI *ui = nullptr;
    using Levels = map<int, Level *>;
    map<string, Levels> regions = {};
    const Uint32 PASSIVE_EVENT = SDL_RegisterEvents(1);
    map<string, vector<int>> lvlNums = {};
    map<string, Audio> audios = {};
    Uint64 LAST = SDL_GetPerformanceCounter();
    map<string, int> rgnMaxLvls = {};
    string crntRgnName = "";
    int lvlNum = 0;
    Uint64 NOW;
    void updateDeltaTime();
    void collision();
    void nextLevel();
    static Uint32 passiveTimerCallback(void *userData, SDL_TimerID id, Uint32 interval);
};