#pragma once

#include "../../engine/map.h"
#include "../group.h"
#include "entities/player.h"
#include "entities/objects/fruit.h"
#include "entities/objects/spike.h"
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
    Player *player;
    Checkpoint checkpoint;
    vector<Map *> maps = {};
    string region = "", seed = "";
    struct MapGroup
    {
        World *world = nullptr;
        vector<Object> objects;
        Group<Spike> spikes;
        Group<Pad> pads;
        Group<Fruit> fruits;
        Group<Enemy> enemies;
        using EnemyFactory = std::function<unique_ptr<Enemy>()>;
        enum SpriteTypes
        {
            object,
            spike,
            pad,
            fruit,
            enemy,
            slime,
            bee,
            snail
        };
        int width, height, pixelWidth, pixelHeight;
        MapGroup(Player *player);
        void handle(double dt);
        void render(Vector2D Camera);
        void loadObjects(Map *newMap);
        void clear();
        SpriteTypes hashString(string str);
    } map;
    std::map<string, Quest> quests = {};
    std::map<string, Audio> audios = {};
    int fruitLength = 0, enemyLength = 0;
    double increment = 0.0;
    World(SDL_Renderer *renderer, Player *player, string region, string seed);
    void handle(double dt);
    void render();
    void loadMaps();
    void reset();
    void clampCamera();
};