#pragma once

#include "tools/sprites/weapon.h"

class Sting : public Weapon
{
public:
    Sting(SDL_Renderer *renderer, float x, float y, Vector2D Direction);
};