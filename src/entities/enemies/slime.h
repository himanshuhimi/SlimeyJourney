#pragma once

#include "tools/sprites/enemy.h"
#include "../weapons/ball.h"

class Slime : public Enemy
{
public:
    map<string, Animation> anims = {};
    vector<Ball> balls = {};
    Slime(SDL_Renderer *renderer, float x, float y);
    void handle(double dt, const vector<Object> &objects) override;
    void render(Vector2D Camera) override;
    void attack(Vector2D Direction);
};