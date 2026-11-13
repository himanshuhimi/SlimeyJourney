#pragma once

#include "../../engine/map.h"
#include "../group.h"
#include "entities/player.h"
#include "entities/objects/fruit.h"
#include "entities/objects/spike.h"
#include "entities/objects/stone.h"
#include "entities/objects/pad.h"
#include "entities/passives/checkpoint.h"
#include "entities/enemies/slime.h"
#include "entities/enemies/snail.h"
#include "entities/enemies/bee.h"
#include "tools/systems/quest.h"

struct World
{
    SDL_Renderer *renderer = nullptr;
    Vector2D Camera;
    Player player;
    Checkpoint checkpoint;
    vector<Map *> maps = {};
    string region = "", seed = "";
    struct MapGroup
    {
        World *world = nullptr;
        int width, height, pixelWidth, pixelHeight;
        vector<Object> objects = {};
        Group<Spike> spikes = {};
        Group<Stone> stones = {};
        Group<Pad> pads = {};
        Group<Fruit> fruits = {};
        Group<Enemy> enemies = {};
        void handle(double dt);
        void render(Vector2D Camera);
        void clear();
    } map;
    std::map<string, Quest> quests = {};
    std::map<string, Audio> audios = {};
    using EnemyFactory = std::function<unique_ptr<Enemy>()>;
    int fruitLength = 0, enemyLength = 0;
    double increment = 0.0;
    World(SDL_Renderer *renderer, Player player, string region, string seed);
    void handle(double dt);
    void render();
    void loadMaps();
    void loadObjects(Map *newMap, int offset);
    void reset();
    void clampCamera();
};