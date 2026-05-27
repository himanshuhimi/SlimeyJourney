#pragma once

#include "map.h"
#include "../sprites/grass.h"
#include "../sprites/player.h"
#include "../sprites/fruit.h"

class Level
{
public:
    int points = 0;
    Player player;
    vector<Grass> grasses;
    vector<Fruit> fruits;
    Map map;
    Vector2D Camera;
    Level(SDL_Renderer *renderer, int number);
    void render();
    void handle(double dt);
};