#pragma once

#include "map.h"
#include "ui/progress.h"
#include "sprites/player.h"
#include "sprites/fruit.h"
#include "sprites/flag.h"
#include "sprites/enemies/slime.h"
#include "sprites/heart.h"

class Level
{
public:
    SDL_Renderer *renderer = nullptr;
    Progress fruitBar;
    Vector2D Camera;
    Map map;
    Flag flag;
    Player player;
    Timer timer;
    vector<Object> grasses = {};
    vector<Fruit> fruits = {};
    vector<Slime> slimes = {};
    vector<Heart> hearts = {};
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