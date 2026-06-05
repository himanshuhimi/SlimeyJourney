#pragma once

#include "map.h"
#include "../sprites/grass.h"
#include "../sprites/player.h"
#include "../sprites/fruit.h"
#include "../sprites/slime.h"
#include "../ui/progress.h"

class Level
{
public:
    SDL_Renderer *renderer = nullptr;
    Progress fruitBar;
    Vector2D Camera;
    Map map;
    Player player;
    vector<Grass> grasses = {};
    vector<Fruit> fruits = {};
    vector<Enemy> enemies = {};
    std::map<string, Audio> audios;
    int points = 0, fruitLength = 0;
    double increment = 0.0;
    Level(SDL_Renderer *renderer, int number);
    void handle(double dt);
    void render();
    void collision();
    void loadObjects();
    void clampCamera();
};