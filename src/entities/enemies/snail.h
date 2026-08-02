#pragma once

#include "tools/sprites/enemy.h"
#include "../weapons/spit.h"

class Snail : public Enemy
{
public:
    bool shelled = false;
    vector<Spit> spits = {};
    Snail(SDL_Renderer *renderer, float x, float y);
    void attack(Vector2D Direction);
};