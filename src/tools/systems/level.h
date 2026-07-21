#pragma once

#include "map.h"
#include "entities/player.h"
#include "entities/fruit.h"
#include "entities/flag.h"
#include "entities/fren.h"
#include "entities/spike.h"
#include "entities/stone.h"
#include "entities/enemies/slime.h"
#include "tools/systems/quest.h"

class Level
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Camera;
    Map map;
    Player player;
    Fren fren;
    Timer timer;
    vector<Object> objects = {};
    vector<Spike> spikes = {};
    vector<Stone> stones = {};
    vector<unique_ptr<Fruit>> fruits = {};
    vector<unique_ptr<Slime>> enemies = {};
    std::map<string, Quest> quests = {};
    std::map<string, Audio> audios;
    int fruitLength = 0;
    double increment = 0.0;
    Level(SDL_Renderer *renderer, string region, int number);
    void handle(double dt);
    void render();
    void loadObjects();
    void reset();
    void clampCamera();
};