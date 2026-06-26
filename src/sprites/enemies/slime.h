#pragma once

#include "../ball.h"
#include "tools/enemy.h"

class Slime : public Enemy
{
public:
    map<string, Animation> anims = {};
    vector<Ball> balls = {};
    Slime(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects);
    void render(Vector2D Camera);
    void attack(Vector2D Direction);
    void damage(int byPoints = 1);
};