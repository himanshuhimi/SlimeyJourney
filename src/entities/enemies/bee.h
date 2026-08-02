#pragma once

#include "tools/sprites/enemy.h"
#include "../weapons/sting.h"

class Bee : public Enemy
{
public:
    vector<Sting> stings = {};
    Bee(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
    void attack(Vector2D Direction);
};