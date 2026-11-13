#pragma once

#include "tools/sprites/enemy.h"
#include "../weapons/sting.h"

struct Bee : public Enemy
{

    Bee(SDL_Renderer *renderer, float x, float y);
    void attack(Vector2D Direction);
};