#pragma once

#include "tools/sprites/enemy.h"
#include "../weapons/sting.h"

class Bee : public Enemy
{
public:
    Bee(SDL_Renderer *renderer, float x, float y);
    void attack(Vector2D Direction);
};