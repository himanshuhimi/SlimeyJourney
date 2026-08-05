#pragma once

#include "map.h"
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
    Timer timer;
    vector<Object> objects = {};
    vector<Spike> spikes = {};
    vector<Stone> stones = {};
    vector<Pad> pads = {};
    vector<unique_ptr<Fruit>> fruits = {};
    vector<unique_ptr<Enemy>> enemies = {};
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