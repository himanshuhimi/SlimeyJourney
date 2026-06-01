#pragma once

#include "map.h"
#include "../sprites/grass.h"
#include "../sprites/player.h"
#include "../sprites/fruit.h"
#include "../sprites/slime.h"

class Level
{
public:
    int points = 0;
    SDL_Renderer *renderer = nullptr;
    Map map;
    Vector2D Camera;
    Text pointsText;
    Image bottleImage;
    SDL_FRect bottleRect;
    Player player;
    vector<Grass> grasses;
    vector<Fruit> fruits;
    vector<Enemy> enemies;
    Level(SDL_Renderer *renderer, int number);
    void render();
    void handle(double dt);
};