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
    Player player;
    vector<Grass> grasses = {};
    vector<Fruit> fruits = {};
    vector<Enemy> enemies = {};
    Progress fruitBar;
    int points = 0, fruitLength = 0;
    double increment = 0.0;
    Level(SDL_Renderer *renderer, int number);
    void handle(double dt);
    void render();
    void loadObjects();
    void clampCamera();

private:
    Map map;
    Vector2D Camera;
};