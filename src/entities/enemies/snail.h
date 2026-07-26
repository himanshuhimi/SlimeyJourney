#pragma once

#include "tools/enemy.h"

class Snail : public Enemy
{
public:
    Snail(SDL_Renderer *renderer, float x, float y);
    void attack(Vector2D Direction);
};