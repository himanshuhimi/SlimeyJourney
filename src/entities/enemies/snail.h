#pragma once

#include "tools/sprites/enemy.h"

class Snail : public Enemy
{
public:
    bool shelled = false;
    Snail(SDL_Renderer *renderer, float x, float y);
    void attack(Vector2D Direction);
};