#pragma once

#include "../ball.h"
#include "tools/enemy.h"

class Slime : public Enemy
{
public:
    vector<Ball> balls = {};
    Slime(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &grasses);
    void attack(Vector2D Direction);
};