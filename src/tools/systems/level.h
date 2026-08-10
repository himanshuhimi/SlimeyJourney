#pragma once

#include "../../engine/map.h"
#include "../group.h"
#include "entities/player.h"
#include "entities/objects/fruit.h"
#include "entities/objects/spike.h"
#include "entities/objects/stone.h"
#include "entities/objects/pad.h"
#include "entities/passives/fren.h"
#include "entities/enemies/slime.h"
#include "entities/enemies/snail.h"
#include "entities/enemies/bee.h"
#include "tools/systems/quest.h"

class Level
{
public:
    SDL_Renderer *renderer = nullptr;
    Vector2D Camera;
    Map map;
    Player player;
    Fren fren;
    vector<Object> objects = {};
    Group<Spike> spikes = {};
    Group<Stone> stones = {};
    Group<Pad> pads = {};
    Group<Fruit> fruits = {};
    Group<Enemy> enemies = {};
    std::map<string, Quest> quests = {};
    std::map<string, Audio> audios = {};
    using EnemyFactory = std::function<unique_ptr<Enemy>()>;
    int fruitLength = 0, enemyLength = 0;
    double increment = 0.0;
    Level(SDL_Renderer *renderer, string region, int number);
    void handle(double dt);
    void render();
    void loadObjects();
    void reset();
    void clampCamera();
};