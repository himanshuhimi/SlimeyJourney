#pragma once

#include "tools/sprites/weapon.h"

class Spit : public Weapon
{
public:
    Spit(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
};