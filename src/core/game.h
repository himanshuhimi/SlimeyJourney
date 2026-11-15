#pragma once

#include "config.h"
#include "tools/systems/settings.h"
#include "tools/systems/world.h"
#include "tools/systems/region.h"
#include "entities/passives/cloud.h"
#include "entities/passives/petal.h"
#include "tools/systems/ui.h"
#include "tools/parallax.h"
#include "tools/interval.h"

struct UI;
struct Game
{

    SDL_Renderer *renderer = nullptr;
    SDL_Window *window = nullptr;
    SDL_Event event;
    Settings *settings = nullptr;
    Player player;
    Scenes scene = Scenes::HOME, prevScene = Scenes::HOME, nextScene = Scenes::HOME;
    World *world = nullptr;
    bool active = false;
    double dt = 0.0;
    string worldSeed = "";
    Game();
    void launch();
    void render();
    void handle();
    void setScene(Scenes newState, bool loading = true);
    void terminate();
    void loadWorld(string region);
    void unloadWorld();

private:
    vector<Cloud> clouds = {};
    vector<Petal> petals = {};
    Parallax *parallax;
    UI *ui = nullptr;
    map<string, Audio> audios = {};
    Uint64 LAST = SDL_GetPerformanceCounter();
    Uint64 NOW;
    Interval *passiveTimer;
    SDL_Color background;
    int realHour = 0;
    void updateDeltaTime();
    void handleEnemies();
    void handleObjects();
    void handlePassives();
    void collision();
    int getTime();
};