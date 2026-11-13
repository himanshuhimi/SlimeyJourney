#pragma once

#include "tools/sprites/weapon.h"

struct Spit : public Weapon
{

    Spit(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
};