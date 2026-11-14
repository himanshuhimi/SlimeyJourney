#pragma once

#include "tools/sprites/weapon.h"

struct Sting : public Weapon
{
    Sting(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
};